#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082
#define BUFFER_SIZE 1024

// Function to perform mathematical operations
double perform_operation(const char *operation, double a, double b, int *error) {
    *error = 0; // No error by default

    if (strcmp(operation, "add") == 0) {
        return a + b;
    } else if (strcmp(operation, "subtract") == 0) {
        return a - b;
    } else if (strcmp(operation, "multiply") == 0) {
        return a * b;
    } else if (strcmp(operation, "divide") == 0) {
        if (b == 0) {
            *error = 1; // Division by zero error
            return 0;
        }
        return a / b;
    } else {
        *error = 2; // Invalid operation error
        return 0;
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept client connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Receive request from client
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(new_socket, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Received request: %s\n", buffer);

        // Parse request
        char operation[BUFFER_SIZE];
        double a, b;
        int error;
        if (sscanf(buffer, "%s %lf %lf", operation, &a, &b) != 3) {
            snprintf(buffer, BUFFER_SIZE, "Error: Invalid request format.");
        } else {
            // Perform operation
            double result = perform_operation(operation, a, b, &error);

            if (error == 1) {
                snprintf(buffer, BUFFER_SIZE, "Error: Division by zero.");
            } else if (error == 2) {
                snprintf(buffer, BUFFER_SIZE, "Error: Invalid operation.");
            } else {
                snprintf(buffer, BUFFER_SIZE, "Result: %.2f", result);
            }
        }

        // Send response to client
        send(new_socket, buffer, strlen(buffer), 0);
    }

    // Close sockets
    close(new_socket);
    close(server_fd);

    return 0;
}
