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
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    char operation[10], filename[BUFFER_SIZE], content[BUFFER_SIZE];
    memset(content, 0, BUFFER_SIZE);

    printf("Enter operation (CREATE/READ/DELETE): ");
    scanf("%s", operation);

    printf("Enter filename: ");
    scanf("%s", filename);

    if (strcmp(operation, "CREATE") == 0) {
        printf("Enter file content: ");
        getchar(); // Clear newline
        fgets(content, BUFFER_SIZE, stdin);
    }

    snprintf(buffer, BUFFER_SIZE, "%s %s \"%s\"", operation, filename, content);
    send(client_fd, buffer, strlen(buffer), 0);

    memset(buffer, 0, BUFFER_SIZE);
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Server response: %s\n", buffer);

    close(client_fd);
    return 0;
}
