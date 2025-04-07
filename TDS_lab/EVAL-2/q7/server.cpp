#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8082
#define BUFFER_SIZE 1024

void send_file(int client_socket, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        const char *error_msg = "ERROR: File not found";  // Changed to const char*
        send(client_socket, error_msg, strlen(error_msg), 0);
        return;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Send file size first
    send(client_socket, &file_size, sizeof(file_size), 0);

    // Send file content in chunks
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(file);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char filename[256];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept client connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Receive filename
        int bytes_received = recv(client_socket, filename, sizeof(filename) - 1, 0);
        if (bytes_received > 0) {
            filename[bytes_received] = '\0';
            printf("Requested file: %s\n", filename);
            send_file(client_socket, filename);
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}