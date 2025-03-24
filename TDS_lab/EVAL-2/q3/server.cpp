// Server Side (server.c)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8082
#define BUFFER_SIZE 1024

void handle_request(int client_socket)
{
    char buffer[BUFFER_SIZE];
    int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
    if (bytes_read <= 0) {
        perror("Read failed");
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';
    char operation[10], filename[100], content[BUFFER_SIZE];
    sscanf(buffer, "%s %s %[^\n]", operation, filename, content);

    if (strcmp(operation, "CREATE") == 0) {
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            write(client_socket, "Failed to create file", 22);
        } else {
            fprintf(file, "%s", content);
            fclose(file);
            write(client_socket, "File created successfully", 24);
        }
    } else if (strcmp(operation, "READ") == 0) {
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            write(client_socket, "File not found", 15);
        } else {
            char read_buffer[BUFFER_SIZE];
            fread(read_buffer, 1, BUFFER_SIZE, file);
            fclose(file);
            write(client_socket, read_buffer, strlen(read_buffer));
        }
    } else if (strcmp(operation, "DELETE") == 0) {
        if (remove(filename) == 0) {
            write(client_socket, "File deleted successfully", 25);
        } else {
            write(client_socket, "File deletion failed", 21);
        }
    } else {
        write(client_socket, "Invalid operation", 18);
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        handle_request(client_socket);
    }

    close(server_fd);
    return 0;
}
