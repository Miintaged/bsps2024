#ifndef MQ_FUNCTIONS_H
#define MQ_FUNCTIONS_H

#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>

void mp_open_safe(const char* path, mqd_t* msq, int o_flags) {
    *msq = mq_open(path, o_flags, 0666);
    if (*msq == -1) {
        perror("mq open");
        exit(EXIT_FAILURE);
    }
}

ssize_t mq_receive_safe(mqd_t msq, char* buffer, size_t size, unsigned int* priority) {
    ssize_t bytes_received = mq_receive(msq, buffer, size, priority);
    if (bytes_received == -1) {
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }
    return bytes_received;
}

ssize_t mq_send_safe(mqd_t msq, const char* buffer, size_t size, unsigned int priority) {
    ssize_t bytes_sent = mq_send(msq, buffer, size, priority);
    if (bytes_sent == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
    return bytes_sent;
}

void init_attr(struct mq_attr* mqAttr, int max_messages, int max_message_sizes) {
    mqAttr->mq_msgsize = max_message_sizes;
    mqAttr->mq_maxmsg = max_messages;
    mqAttr->mq_flags = 0;
    mqAttr->mq_curmsgs = 0;
}

void mq_clear(mqd_t msq, struct mq_attr* mqAttr) {
    mq_getattr(msq, mqAttr);
    while (mqAttr->mq_curmsgs) {
        mq_receive_safe(msq, NULL, mqAttr->mq_msgsize, NULL);
        mq_getattr(msq, mqAttr);
    }
}

_Bool mq_is_empty(mqd_t msq, struct mq_attr* mqAttr) {
    mq_getattr(msq, mqAttr);
    return mqAttr->mq_curmsgs == 0;
}

#endif