/* Define the program number and version */
/* Define the message structure */
struct Message {
    string sender<100>; 
    string content<500>;
};

/* Define the message queue */
typedef Message MessageQueue<100>; 
struct MessageQueue{
    Message messages[100];
};

/* Define the procedures */
program CHAT_PROG {
    version CHAT_VERS {
        /* Send a message to the server */
        void SEND_MESSAGE(Message) = 1;

        /* Retrieve all messages from the server */
        MessageQueue GET_MESSAGES(void) = 2;
    } = 1;
} = 0x20000001;
