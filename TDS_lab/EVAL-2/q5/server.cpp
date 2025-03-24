// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8082
#define BUFFER_SIZE 1024
#define LOG_FILE "logs.txt"

void log_message(const char *log_type, const char *message) {
    FILE *file = fopen(LOG_FILE, "a");
    if (!file) {
        perror("Failed to open log file");
        return;
    }

    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0'; // Remove newline
	printf("Timestamp: %s\nLog Type: %s\nMessage: %s\n", timestamp, log_type, message);

    fprintf(file, "%s [%s]: %s\n", timestamp, log_type, message);
    fclose(file);
}

void send_logs(int client_socket, const char *filter) {
    FILE *file = fopen(LOG_FILE, "r");
    if (!file) {
        perror("Failed to open log file");
        return;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, filter) || strcmp(filter, "ALL") == 0) {
            send(client_socket, line, strlen(line), 0);
        }
    }
    fclose(file);
    send(client_socket, "END_OF_LOGS", 12, 0);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        recv(client_socket, buffer, sizeof(buffer), 0);
        char type[10], message[BUFFER_SIZE];
        sscanf(buffer, "%s %[^\n]", type, message);

        if (strcmp(type, "GET") == 0) {
            send_logs(client_socket, message);
        } else {
            log_message(type, message);
            send(client_socket, "Log saved successfully", 22, 0);
        }
        close(client_socket);
    }

    return 0;
}

