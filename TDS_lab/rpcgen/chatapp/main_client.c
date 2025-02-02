#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chat_prog_1(char *host) {
    CLIENT *clnt;
    void *result_1;
    Message send_message_1_arg;
    MessageQueue *result_2;
    char *get_messages_1_arg;

    clnt = clnt_create(host, CHAT_PROG, CHAT_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    // Send a message
    printf("Enter sender name: ");
    fgets(send_message_1_arg.sender, sizeof(send_message_1_arg.sender), stdin);
    send_message_1_arg.sender[strcspn(send_message_1_arg.sender, "\n")] = 0;

    printf("Enter message: ");
    fgets(send_message_1_arg.content, sizeof(send_message_1_arg.content), stdin);
    send_message_1_arg.content[strcspn(send_message_1_arg.content, "\n")] = 0;

    result_1 = send_message_1(&send_message_1_arg, clnt);
    if (result_1 == (void *)NULL) {
        clnt_perror(clnt, "call failed");
    } else {
        printf("Message sent successfully.\n");
    }

    // Retrieve messages
    result_2 = get_messages_1((void *)&get_messages_1_arg, clnt);
    if (result_2 == (MessageQueue *)NULL) {
        clnt_perror(clnt, "call failed");
    } else {
        printf("Message history:\n");
        for (int i = 0; i < 100 && result_2->messages[i].sender[0] != '\0'; i++) {
            printf("[%s]: %s\n", result_2->messages[i].sender, result_2->messages[i].content);
        }
    }

    clnt_destroy(clnt);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s server_host\n", argv[0]);
        exit(1);
    }
    chat_prog_1(argv[1]);
    return 0;
}
