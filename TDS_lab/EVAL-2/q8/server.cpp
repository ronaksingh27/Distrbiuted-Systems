#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define MAX_KEYS 1000

// Key-value pair structure
typedef struct {
    char key[256];
    char value[256];
    int in_use;
} KeyValue;

// Shared key-value store
KeyValue store[MAX_KEYS];
pthread_mutex_t store_mutex = PTHREAD_MUTEX_INITIALIZER;

void init_store() {
    for (int i = 0; i < MAX_KEYS; i++) {
        store[i].in_use = 0;
    }
}

void *handle_client(void *arg) {
    int client_sock = *(int*)arg;
    char buffer[BUFFER_SIZE];
    free(arg);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            close(client_sock);
            return NULL;
        }

        char command[10], key[256], value[256];
        sscanf(buffer, "%s %s %s", command, key, value);

        pthread_mutex_lock(&store_mutex);

        if (strcmp(command, "SET") == 0) {
            int i;
            // Look for existing key or empty slot
            for (i = 0; i < MAX_KEYS; i++) {
                if (store[i].in_use && strcmp(store[i].key, key) == 0) {
                    strcpy(store[i].value, value);
                    break;
                }
                if (!store[i].in_use) {
                    strcpy(store[i].key, key);
                    strcpy(store[i].value, value);
                    store[i].in_use = 1;
                    break;
                }
            }
            send(client_sock, i < MAX_KEYS ? "OK\n" : "STORE_FULL\n", 
                 i < MAX_KEYS ? 3 : 11, 0);
        }
        else if (strcmp(command, "GET") == 0) {
            for (int i = 0; i < MAX_KEYS; i++) {
                if (store[i].in_use && strcmp(store[i].key, key) == 0) {
                    char response[BUFFER_SIZE];
                    snprintf(response, BUFFER_SIZE, "%s\n", store[i].value);
                    send(client_sock, response, strlen(response), 0);
                    pthread_mutex_unlock(&store_mutex);
                    goto found;
                }
            }
            send(client_sock, "NOT_FOUND\n", 10, 0);
found:;
        }
        else if (strcmp(command, "DELETE") == 0) {
            for (int i = 0; i < MAX_KEYS; i++) {
                if (store[i].in_use && strcmp(store[i].key, key) == 0) {
                    store[i].in_use = 0;
                    send(client_sock, "OK\n", 3, 0);
                    pthread_mutex_unlock(&store_mutex);
                    goto deleted;
                }
            }
            send(client_sock, "NOT_FOUND\n", 10, 0);
deleted:;
        }
        else {
            send(client_sock, "INVALID_COMMAND\n", 16, 0);
        }

        pthread_mutex_unlock(&store_mutex);
    }
    return NULL;
}

int main() {
    int server_sock, *client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    init_store();

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        *client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (*client_sock < 0) {
            perror("Accept failed");
            free(client_sock);
            continue;
        }

        printf("New client connected\n");

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, client_sock) != 0) {
            perror("Thread creation failed");
            close(*client_sock);
            free(client_sock);
            continue;
        }
        pthread_detach(thread);
    }

    close(server_sock);
    return 0;
}