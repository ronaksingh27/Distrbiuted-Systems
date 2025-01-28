const MAX_DATA = 1024;  // Maximum size for filename and content data

/* Task types for file operations */
enum TaskType {
    CREATE_FILE = 1,
    READ_FILE = 2,
    DELETE_FILE = 3
};

/* Status codes for operations */
enum StatusCode {
    SUCCESS = 0,
    FILE_NOT_FOUND = 1,
    PERMISSION_DENIED = 2,
    UNKNOWN_ERROR = 3
};

/* 
 * FileOperation struct represents a file operation request or response.
 * - task: The type of operation to perform (CREATE_FILE, READ_FILE, DELETE_FILE)
 * - status: The status of the operation (SUCCESS, FILE_NOT_FOUND, etc.)
 * - filename: The name of the file to operate on
 * - content: The content to write or read from the file
 */
struct FileOperation{
    int task;
    int status;
    char filename[MAX_DATA];
    char content[MAX_DATA];
} ;

/*
 * CRUD_PROG program defines the RPC interface for file operations.
 * - CRUD_VERS: The version of the program
 * - createFile: Creates a file with the given content
 * - readFile: Reads the contents of a file
 * - deleteFile: Deletes the contents of a file
 */
program CRUD_PROG {
    version CRUD_VERS {
        FileOperation createFile(FileOperation op) = 1;
        FileOperation readFile(FileOperation op) = 2;
        FileOperation deleteFile(FileOperation op) = 3;
    } = 1;
} = 0x2000001;  // Unique program ID