#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "main.h"

char **reverse_string_1_svc(char **argp, struct svc_req *rqstp) {
    static char *result = NULL;
    char *input = *argp;
    int len = strlen(input);

    if (result) {
        free(result);
    }
    result = (char *)malloc(len + 1);
    if (!result) {
        return NULL;
    }

    for (int i = 0; i < len; i++) {
        result[i] = input[len - 1 - i];
    }
    result[len] = '\0';

    return &result;
}

char **to_uppercase_1_svc(char **argp, struct svc_req *rqstp) {
    static char *result = NULL;
    char *input = *argp;
    int len = strlen(input);

    if (result) {
        free(result);
    }
    result = (char *)malloc(len + 1);
    if (!result) {
        return NULL;
    }

    for (int i = 0; i < len; i++) {
        result[i] = toupper(input[i]);
    }
    result[len] = '\0';

    return &result;
}

bool_t *is_palindrome_1_svc(char **argp, struct svc_req *rqstp) {
    static bool_t result;
    char *input = *argp;
    int len = strlen(input);

    result = TRUE;
    for (int i = 0; i < len / 2; i++) {
        if (input[i] != input[len - 1 - i]) {
            result = FALSE;
            break;
        }
    }

    return &result;
}

