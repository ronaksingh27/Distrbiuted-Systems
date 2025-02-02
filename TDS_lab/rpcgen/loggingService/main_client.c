#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"

void log_prog_1(char *host) {
    CLIENT *clnt;
    ServerResponse *result_1;
    message sendmessagetoserver_1_arg;
    LogResponse *result_2;
    LogRequest getlogs_1_arg;

    clnt = clnt_create(host, LOG_PROG, LOG_VERS, "udp");
    if (!clnt) {
        clnt_pcreateerror(host);
        exit(1);
    }

    /* Sending log message */
    printf("Enter log message: ");
    fgets(sendmessagetoserver_1_arg.logMsg, sizeof(sendmessagetoserver_1_arg.logMsg), stdin);
    sendmessagetoserver_1_arg.logMsg[strcspn(sendmessagetoserver_1_arg.logMsg, "\n")] = '\0';

    printf("Enter log type (1=INFO, 2=WARN, 3=ERROR): ");
    scanf("%d", (int*)&sendmessagetoserver_1_arg.type);
    getchar();

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[100];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    sendmessagetoserver_1_arg.timestamp = timestamp;

    result_1 = sendmessagetoserver_1(&sendmessagetoserver_1_arg, clnt);
    if (!result_1) {
        clnt_perror(clnt, "call failed");
    } else {
        printf("Server response: %s\n", result_1->ack ? "Success" : result_1->errorMsg);
    }

    /* Requesting logs */
    printf("Enter log type to retrieve (0=ALL, 1=INFO, 2=WARN, 3=ERROR): ");
    scanf("%d", (int*)&getlogs_1_arg.type);

    result_2 = getlogs_1(&getlogs_1_arg, clnt);
    if (!result_2) {
        clnt_perror(clnt, "call failed");
    } else {
        printf("\n--- Retrieved Logs ---\n");
        for (int i = 0; i < result_2->logs.logs_len; i++) {
            printf("[%s] [%d] %s\n", result_2->logs.logs_val[i].timestamp, result_2->logs.logs_val[i].type, result_2->logs.logs_val[i].logMsg);
        }
    }

    clnt_destroy(clnt);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s server_host\n", argv[0]);
        exit(1);
    }
    log_prog_1(argv[1]);
    return 0;
}
