#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

void string_ops_prog_1(char *host) {
    CLIENT *clnt;
    char *input_str = (char *)malloc(100);
    int choice;
    
    if (!input_str) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    printf("Enter a string: ");
    fgets(input_str, 100, stdin);
    input_str[strcspn(input_str, "\n")] = '\0';  // Remove newline character

    printf("\nChoose an operation:\n");
    printf("1. Reverse String\n");
    printf("2. Convert to Uppercase\n");
    printf("3. Check if Palindrome\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar();  // Clear the newline from buffer

#ifndef DEBUG
    clnt = clnt_create(host, STRING_OPS_PROG, STRING_OPS_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif /* DEBUG */

    switch (choice) {
        case 1: {
            char **result = reverse_string_1(&input_str, clnt);
            if (result == NULL) {
                clnt_perror(clnt, "RPC call failed");
            } else {
                printf("Reversed String: %s\n", *result);
            }
            break;
        }
        case 2: {
            char **result = to_uppercase_1(&input_str, clnt);
            if (result == NULL) {
                clnt_perror(clnt, "RPC call failed");
            } else {
                printf("Uppercase String: %s\n", *result);
            }
            break;
        }
        case 3: {
            bool_t *result = is_palindrome_1(&input_str, clnt);
            if (result == NULL) {
                clnt_perror(clnt, "RPC call failed");
            } else {
                printf("Palindrome Check: %s\n", (*result) ? "Yes" : "No");
            }
            break;
        }
        default:
            printf("Invalid choice!\n");
            break;
    }

#ifndef DEBUG
    clnt_destroy(clnt);
#endif /* DEBUG */

    free(input_str);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s server_host\n", argv[0]);
        exit(1);
    }
    
    string_ops_prog_1(argv[1]);
    return 0;
}
