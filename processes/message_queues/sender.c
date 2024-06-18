#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <limits.h>


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./service.c \"/csXXXX\"");
        return EXIT_FAILURE;
    }

    mqd_t msq = mq_open(argv[1], O_WRONLY, 0666);
    if(msq == -1) {
        perror("mq open");
        return EXIT_FAILURE;
    }

    unsigned int priority;
    char* ptr;
    priority = strtol(argv[2], &ptr, 10);

    char buffer[PIPE_BUF];
    fgets(buffer, PIPE_BUF, stdin);

    ssize_t bytes_sent = mq_send(msq, buffer, PIPE_BUF, priority);
    if(bytes_sent == -1) {
        perror("mq_send");
        return EXIT_FAILURE;
    }

    mq_close(msq);
    return EXIT_SUCCESS;
}