
// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    int choice;
    printf("1. Send Log\n2. Request Logs\nEnter choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        char type[10], message[BUFFER_SIZE];
        printf("Enter log type (INFO/WARN/ERROR): ");
        scanf("%s", type);
        getchar();
        printf("Enter log message: ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = 0;

        snprintf(buffer, sizeof(buffer), "%s %s", type, message);
        send(client_fd, buffer, strlen(buffer), 0);
    } else if (choice == 2) {
        char filter[10];
        printf("Enter log type to retrieve (INFO/WARN/ERROR/ALL): ");
        scanf("%s", filter);

        snprintf(buffer, sizeof(buffer), "GET %s", filter);
        send(client_fd, buffer, strlen(buffer), 0);

        printf("Logs:\n");
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
            if (bytes_received <= 0 || strcmp(buffer, "END_OF_LOGS") == 0)
                break;
            printf("%s", buffer);
        }
    }

    close(client_fd);
    return 0;
}
