#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server. Commands: SET key value, GET key, DELETE key\n");

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline

        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            perror("Send failed");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            printf("Server disconnected\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("%s", buffer);
    }

    close(sock);
    return 0;
}