/* Define the program number and version */
const CHAT_PROG = 0x20000001;
const CHAT_VERS = 1;

/* Define the message structure */
struct Message {
    string sender<100>;  // Sender's name or ID
    string content<500>; // Message content
};

/* Define the message queue */
typedef Message MessageQueue<100>; // Queue can hold up to 100 messages
struct MessageQueue{
    Message messages[100];
}

/* Define the procedures */
program CHAT_PROG {
    version CHAT_VERS {
        /* Send a message to the server */
        void SEND_MESSAGE(Message) = 1;

        /* Retrieve all messages from the server */
        MessageQueue GET_MESSAGES(void) = 2;
    } = 1;
} = 0x20000001;