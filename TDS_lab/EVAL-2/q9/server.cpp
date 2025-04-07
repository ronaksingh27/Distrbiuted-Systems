#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082
#define BUFFER_SIZE 1024

pthread_mutex_t lock;

void update_votes(const char *candidate) {
    pthread_mutex_lock(&lock);
    FILE *file = fopen("votes.txt", "a+");
    if (file == NULL) {
        perror("File opening failed");
        pthread_mutex_unlock(&lock);
        return;
    }
    fprintf(file, "%s\n", candidate);
    fclose(file);
    pthread_mutex_unlock(&lock);
}

int get_vote_count(const char *candidate) {
    pthread_mutex_lock(&lock);
    FILE *file = fopen("votes.txt", "r");
    if (file == NULL) {
        perror("File opening failed");
        pthread_mutex_unlock(&lock);
        return 0;
    }
    char buffer[100];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;  // Remove newline
        if (strcmp(buffer, candidate) == 0) {
            count++;
        }
    }
    fclose(file);
    pthread_mutex_unlock(&lock);
    return count;
}

void *handle_client(void *arg) {
    int server_fd = *(int *)arg;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    while (1) {
        int recv_len = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len <= 0) continue;

        buffer[recv_len] = '\0';
        char command[10], candidate[100];
        sscanf(buffer, "%s %s", command, candidate);

        if (strcmp(command, "VOTE") == 0) {
            update_votes(candidate);
            sendto(server_fd, "Vote registered", strlen("Vote registered"), 0, (struct sockaddr *)&client_addr, addr_len);
        } else if (strcmp(command, "COUNT") == 0) {
            int count = get_vote_count(candidate);
            char response[100];
            sprintf(response, "Total votes for %s: %d", candidate, count);
            sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);
        }
    }
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr;

    pthread_mutex_init(&lock, NULL);

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);
    pthread_t thread;
    pthread_create(&thread, NULL, handle_client, &server_fd);
    pthread_join(thread, NULL);

    pthread_mutex_destroy(&lock);
    close(server_fd);
    return 0;
}
