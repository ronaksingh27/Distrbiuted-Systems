#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8082
#define BUFFER_SIZE 1024

void receive_file(int socket, const char *filename) {
    char buffer[BUFFER_SIZE];
    long file_size;
    long total_bytes = 0;

    // Receive file size first
    int bytes_received = recv(socket, &file_size, sizeof(file_size), 0);
    if (bytes_received <= 0 || file_size == 0) {
        printf("Error receiving file size\n");
        return;
    }

    // Check for error message
    bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        if (strncmp(buffer, "ERROR", 5) == 0) {
            printf("%s\n", buffer);
            return;
        }
    }

    // Open file for writing
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to create file");
        return;
    }

    // Write initial received data
    fwrite(buffer, 1, bytes_received, file);
    total_bytes += bytes_received;

    // Receive remaining file content
    while (total_bytes < file_size) {
        bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) break;

        fwrite(buffer, 1, bytes_received, file);
        total_bytes += bytes_received;

        // Calculate and display progress
        float progress = (float)total_bytes / file_size * 100;
        printf("\rProgress: %.2f%% (%ld/%ld bytes)", progress, total_bytes, file_size);
        fflush(stdout);
    }

    printf("\nFile transfer completed\n");
    fclose(file);
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char filename[256];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Get filename from user
    printf("Enter filename to request: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    // Send filename to server
    send(client_socket, filename, strlen(filename), 0);

    // Receive file
    receive_file(client_socket, filename);

    close(client_socket);
    return 0;
}