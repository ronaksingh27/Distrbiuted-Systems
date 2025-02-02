#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

/* Function to create a file */
FileOperation *createfile_1_svc(FileOperation *op, struct svc_req *req) {
    static FileOperation res;
    memset(&res, 0, sizeof(res));

    FILE *fp = fopen(op->filename, "w");
    if (!fp) {
        res.status = UNKNOWN_ERROR;
        return &res;
    }

    fwrite(op->content, sizeof(char), strlen(op->content), fp);
    fclose(fp);

    res.status = OP_SUCCESS;
    strcpy(res.filename, op->filename);
    return &res;
}

/* Function to read a file */
FileOperation *readfile_1_svc(FileOperation *op, struct svc_req *req) {
    static FileOperation res;
    memset(&res, 0, sizeof(res));

    FILE *fp = fopen(op->filename, "r");
    if (!fp) {
        res.status = FILE_NOT_FOUND;
        return &res;
    }

    fread(res.content, sizeof(char), 1024, fp);
    fclose(fp);

    res.status = OP_SUCCESS;
    strcpy(res.filename, op->filename);
    return &res;
}

/* Function to delete a file */
FileOperation *deletefile_1_svc(FileOperation *op, struct svc_req *req) {
    static FileOperation res;
    memset(&res, 0, sizeof(res));

    if (remove(op->filename) == 0) {
        res.status = OP_SUCCESS;
    } else {
        res.status = FILE_NOT_FOUND;
    }

    strcpy(res.filename, op->filename);
    return &res;
}
