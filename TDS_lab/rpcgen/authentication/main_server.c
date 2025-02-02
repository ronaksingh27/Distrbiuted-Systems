#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#define USER_FILE "users.txt"

// Function to check if credentials exist
int check_credentials(char *username, char *password) {
    FILE *file = fopen(USER_FILE, "r");
    if (!file) {
        return 0;  // File doesn't exist, meaning no users are registered yet
    }

    char stored_user[1024], stored_pass[1024];
    while (fscanf(file, "%s %s", stored_user, stored_pass) != EOF) {
        if (strcmp(stored_user, username) == 0 && strcmp(stored_pass, password) == 0) {
            fclose(file);
            return 1;  // Valid credentials
        }
    }
    fclose(file);
    return 0;  // Invalid credentials
}

// Function to store new credentials
int store_credentials(char *username, char *password) {
    FILE *file = fopen(USER_FILE, "a");
    if (!file) {
        return 0;  // Failed to open file
    }
    fprintf(file, "%s %s\n", username, password);
    fclose(file);
    return 1;
}

AuthResponse *send_credentials_1_svc(Credentials *argp, struct svc_req *rqstp) {
    static AuthResponse result;

    if (argp->action == 0) {  // Signup
        if (store_credentials(argp->username, argp->password)) {
            result.status = 0;
            strcpy(result.message, "Signup successful");
        } else {
            result.status = 1;
            strcpy(result.message, "Signup failed");
        }
    } else if (argp->action == 1) {  // Login
        if (check_credentials(argp->username, argp->password)) {
            result.status = 0;
            strcpy(result.message, "Login successful");
        } else {
            result.status = 1;
            strcpy(result.message, "Invalid username or password");
        }
    } else {
        result.status = 1;
        strcpy(result.message, "Invalid action");
    }

    return &result;
}
