// Server Code: server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082
#define BUFFER_SIZE 1024

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
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
        socklen_t addrlen = sizeof(address);
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected: %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        int bytes_read = read(new_socket, buffer, BUFFER_SIZE);
        buffer[bytes_read] = '\0';

        printf("Received: %s\n", buffer);
        to_uppercase(buffer);

        send(new_socket, buffer, strlen(buffer), 0);
        printf("Sent: %s\n", buffer);

        close(new_socket);
        printf("Client disconnected\n");
    }

    return 0;
}

