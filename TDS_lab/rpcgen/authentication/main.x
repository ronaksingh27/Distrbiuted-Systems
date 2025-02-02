struct Credentials{
    int action;/* 0->signup , 1-> login*/
    char username[1024];
    char password[1024];
};

struct AuthResponse {
    int status; /* 0 -> success, 1 -> failure */
    char message[1024];
};

program AUTH_PROG{
    version AUTH_VERS{
         AuthResponse send_credentials(Credentials c) = 1;
    } = 1;
} = 0x2000001;
