#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

void auth_prog_1(char *host, char *username, char *password, int action) {
    CLIENT *clnt;
    AuthResponse *result_1;
    Credentials send_credentials_1_arg;

    // Set credentials
    send_credentials_1_arg.action = action;
    strncpy(send_credentials_1_arg.username, username, sizeof(send_credentials_1_arg.username) - 1);
    strncpy(send_credentials_1_arg.password, password, sizeof(send_credentials_1_arg.password) - 1);

#ifndef DEBUG
    clnt = clnt_create(host, AUTH_PROG, AUTH_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif /* DEBUG */

    result_1 = send_credentials_1(&send_credentials_1_arg, clnt);
    if (result_1 == (AuthResponse *)NULL) {
        clnt_perror(clnt, "call failed");
    } else {
        printf("Server Response: %s\n", result_1->message);
    }

#ifndef DEBUG
    clnt_destroy(clnt);
#endif /* DEBUG */
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <server_host> <username> <password> <action (0-signup, 1-login)>\n", argv[0]);
        exit(1);
    }

    char *host = argv[1];
    char *username = argv[2];
    char *password = argv[3];
    int action = atoi(argv[4]);

    auth_prog_1(host, username, password, action);
    return 0;
}
