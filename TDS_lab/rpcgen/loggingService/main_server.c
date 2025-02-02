#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"

#define LOG_FILE "server_logs.txt"

ServerResponse *sendmessagetoserver_1_svc(message *argp, struct svc_req *rqstp) {
    static ServerResponse result;
    FILE *file = fopen(LOG_FILE, "a");

    if (!file) {
        result.ack = 0;
        result.errorMsg = strdup("Failed to open log file");
        return &result;
    }

    fprintf(file, "[%s] [%d] %s\n", argp->timestamp, argp->type, argp->logMsg);
    fclose(file);

    result.ack = 1;
    result.errorMsg = strdup("");
    return &result;
}

LogResponse *getlogs_1_svc(LogRequest *argp, struct svc_req *rqstp) {
    static LogResponse result;
    FILE *file = fopen(LOG_FILE, "r");

    if (!file) {
        result.logs.logs_len = 0;
        return &result;
    }

    result.logs.logs_len = 0;
    char buffer[1100];
    while (fgets(buffer, sizeof(buffer), file)) {
        message log_entry;
        char logMsg[1024], timestamp[100];
        int type;

        if (sscanf(buffer, "[%99[^]]] [%d] %1023[^\n]", timestamp, &type, logMsg) == 3) {
            if (argp->type == 0 || argp->type == type) {
                strncpy(log_entry.logMsg, logMsg, sizeof(log_entry.logMsg));
                log_entry.type = type;
                log_entry.timestamp = strdup(timestamp);
                result.logs.logs_val = realloc(result.logs.logs_val, (result.logs.logs_len + 1) * sizeof(message));
                result.logs.logs_val[result.logs.logs_len++] = log_entry;
            }
        }
    }

    fclose(file);
    return &result;
}
