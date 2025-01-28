const int MAX_DATA = 1024;

struct Credentials{
    int action;/* 0->signup , 1-> login*/
    char username[MAX_DATA];
    char password[MAX_DATA];
};

struct AuthResponse {
    int status; // 0 -> success, 1 -> failure
    char message[MAX_DATA];
};

program AUTH_PROG{
    version AUTH_VERS{
         AuthResponse send_credentials(Credentials c) = 1;
    } = 1;
} = 0x2000001;