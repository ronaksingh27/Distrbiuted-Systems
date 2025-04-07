#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8082
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];

    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    while (1) {
        printf("\nEnter command (VOTE <candidate> / COUNT <candidate> / EXIT): ");
        fgets(command, BUFFER_SIZE, stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "EXIT") == 0) {
            break;
        }

        sendto(client_fd, command, strlen(command), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        socklen_t addr_len = sizeof(server_addr);

        int recv_len = recvfrom(client_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
        if (recv_len < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[recv_len] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(client_fd);
    return 0;
}
