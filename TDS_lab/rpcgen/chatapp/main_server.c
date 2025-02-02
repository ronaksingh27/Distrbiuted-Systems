#include "main.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX_MESSAGES 100

// Global message queue
static Message messageQueue[MAX_MESSAGES];
static int messageCount = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *send_message_1_svc(Message *msg, struct svc_req *rqstp) {
    static char *result;
    
    pthread_mutex_lock(&lock);
    
    if (messageCount < MAX_MESSAGES) {
        messageQueue[messageCount] = *msg; // Store the message
        messageCount++;
        result = "Message received";
    } else {
        result = "Message queue full";
    }

    pthread_mutex_unlock(&lock);

    return (void *)&result;
}

MessageQueue *get_messages_1_svc(void *argp, struct svc_req *rqstp) {
    static MessageQueue result;
    
    pthread_mutex_lock(&lock);

    // Copy messages to result queue
    for (int i = 0; i < messageCount; i++) {
        result.messages[i] = messageQueue[i];
    }
    
    pthread_mutex_unlock(&lock);
    
    return &result;
}
