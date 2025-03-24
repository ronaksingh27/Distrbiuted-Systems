#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the array to send
    int arr[] = {10, 20, 30, 40, 50};
    int size = sizeof(arr) / sizeof(arr[0]);
    char serialized_data[BUFFER_SIZE] = "";

    // Serialize array as a comma-separated string
    for (int i = 0; i < size; i++) {
        char num_str[10];
        snprintf(num_str, sizeof(num_str), "%d", arr[i]);
        strcat(serialized_data, num_str);
        if (i < size - 1) {
            strcat(serialized_data, ",");
        }

    }

    printf("serailized data : %s\n",serialized_data);

    // Send data to server
    send(client_fd, serialized_data, strlen(serialized_data), 0);
    printf("Sent array: %s\n", serialized_data);

    // Receive result from server
    memset(buffer, 0, BUFFER_SIZE);
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Received average from server: %s\n", buffer);

    // Close socket
    close(client_fd);

    return 0;
}
