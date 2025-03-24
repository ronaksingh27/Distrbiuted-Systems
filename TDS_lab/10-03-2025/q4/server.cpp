#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082
#define BUFFER_SIZE 1024

// Simple in-memory credential store (username:password)
struct Credential {
    char username[50];
    char password[50];
};

struct Credential valid_creds[] = {
    {"admin", "pass123"},
    {"user1", "secret"}
};
int num_creds = 2;

// Function to validate credentials
int validate_credentials(char *username, char *password) {
    for (int i = 0; i < num_creds; i++) {
        if (strcmp(valid_creds[i].username, username) == 0 &&
            strcmp(valid_creds[i].password, password) == 0) {
            return 1; // Valid
        }
    }
    return 0; // Invalid
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Read the incoming message
        read(new_socket, buffer, BUFFER_SIZE);
        
        // Parse username and password (format: "username:password:message")
        char username[50], password[50], message[BUFFER_SIZE];
        sscanf(buffer, "%[^:]:%[^:]:%s", username, password, message);

        // Validate credentials
        if (validate_credentials(username, password)) {
            printf("Authenticated request from %s: %s\n", username, message);
            char response[BUFFER_SIZE];
            snprintf(response, BUFFER_SIZE, "Request processed successfully for %s", username);
            send(new_socket, response, strlen(response), 0);
        } else {
            const char *error_msg = "Authentication failed: Invalid credentials";
            send(new_socket, error_msg, strlen(error_msg), 0);
            printf("Authentication failed for %s\n", username);
        }

        // Clear buffer and close the connection
        memset(buffer, 0, BUFFER_SIZE);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}