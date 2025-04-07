// client.c - Distributed Matrix Multiplication Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082
#define MAX_SIZE 100

int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];
int rowsA, colsA, rowsB, colsB;

// Send matrix to server
void sendMatrix(int sockfd, int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    write(sockfd, &rows, sizeof(int));
    write(sockfd, &cols, sizeof(int));
    write(sockfd, matrix, rows * cols * sizeof(int));
}

// Receive matrix from server
void receiveMatrix(int sockfd, int matrix[MAX_SIZE][MAX_SIZE], int* rows, int* cols) {
    read(sockfd, rows, sizeof(int));
    read(sockfd, cols, sizeof(int));
    read(sockfd, matrix, (*rows) * (*cols) * sizeof(int));
}

// Input matrix
void inputMatrix(int matrix[MAX_SIZE][MAX_SIZE], int* rows, int* cols) {
    printf("Enter rows and columns: ");
    scanf("%d %d", rows, cols);
    printf("Enter matrix elements:\n");
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

// Print matrix
void printMatrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Input matrices A and B
    printf("Matrix A\n");
    inputMatrix(A, &rowsA, &colsA);

    printf("Matrix B\n");
    inputMatrix(B, &rowsB, &colsB);

    if (colsA != rowsB) {
        printf("Matrix dimensions invalid for multiplication.\n");
        close(sock);
        return 1;
    }

    // Send matrices to server
    sendMatrix(sock, A, rowsA, colsA);
    sendMatrix(sock, B, rowsB, colsB);

    // Receive result matrix from server
    receiveMatrix(sock, C, &rowsA, &colsB);

    printf("Result Matrix:\n");
    printMatrix(C, rowsA, colsB);

    close(sock);
    return 0;
}
