// server.c - Distributed Matrix Multiplication Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8082
#define MAX_SIZE 100
#define MAX_WORKERS 8

int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];
int rowsA, colsA, rowsB, colsB;

// Worker function to compute a row
void* multiplyRow(void* arg) {
    int row = *(int*)arg;
    for (int j = 0; j < colsB; j++) {
        C[row][j] = 0;
        for (int k = 0; k < colsA; k++) {
            C[row][j] += A[row][k] * B[k][j];
        }
    }
    return NULL;
}

// Receive matrix from client
void receiveMatrix(int sockfd, int matrix[MAX_SIZE][MAX_SIZE], int* rows, int* cols) {
    read(sockfd, rows, sizeof(int));
    read(sockfd, cols, sizeof(int));
    read(sockfd, matrix, (*rows) * (*cols) * sizeof(int));
}

// Send matrix to client
void sendMatrix(int sockfd, int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    write(sockfd, &rows, sizeof(int));
    write(sockfd, &cols, sizeof(int));
    write(sockfd, matrix, rows * cols * sizeof(int));
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    printf("Server listening on port %d\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    // Receive matrices A and B
    receiveMatrix(client_fd, A, &rowsA, &colsA);
    receiveMatrix(client_fd, B, &rowsB, &colsB);

    if (colsA != rowsB) {
        printf("Matrix dimensions invalid for multiplication\n");
        close(client_fd);
        return 1;
    }

    pthread_t threads[MAX_SIZE];
    int rowIndices[MAX_SIZE];

    for (int i = 0; i < rowsA; i++) {
        rowIndices[i] = i;
        pthread_create(&threads[i], NULL, multiplyRow, &rowIndices[i]);
    }

    for (int i = 0; i < rowsA; i++) {
        pthread_join(threads[i], NULL);
    }

    // Send result back to client
    sendMatrix(client_fd, C, rowsA, colsB);

    printf("Result sent to client.\n");
    close(client_fd);
    close(server_fd);

    return 0;
} 
