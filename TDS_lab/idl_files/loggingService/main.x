const int MAX_DATA = 1024;

/* Define log types */
enum log_type {
    INFO = 1,
    WARN = 2,
    ERROR = 3
};

/* Message structure with log type and timestamp */
struct message {
    char logMsg[MAX_DATA]; /* Log message content */
    log_type type;         /* Log type (INFO, WARN, ERROR) */
    string timestamp<>;    /* Timestamp as a string */
};

/* Server response structure */
struct ServerResponse {
    int ack;               /* 0 -> failure, 1 -> success */
    string errorMsg<>;     /* Optional error message */
};

/* Structure to request logs */
struct LogRequest {
    log_type type;         /* Requested log type (0 for all logs) */
};

/* Structure to return logs */
struct LogResponse {
    message logs<>;        /* Array of log messages */
};

/* RPC program definition */
program LOG_PROG {
    version LOG_VERS {
        /* Send a log message to the server */
        ServerResponse sendMessageToServer(message m) = 1;

        /* Request logs from the server */
        LogResponse getLogs(LogRequest request) = 2;
    } = 1;
} = 0x2000001;