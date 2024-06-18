#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>

#define MAX_MESSAGES 10

typedef struct messageT {
    unsigned int priority;
    char buffer[PIPE_BUF];
}messageT;

volatile sig_atomic_t runner = true;

void signalHandler() {
    char* message = "\nShutting down.\n";
    write(STDOUT_FILENO, message, strlen(message));
    runner = false;
}

size_t stringToInt(char* string, int* intSequence) {
    char* end_ptr = string;
    size_t number_of_elements = 0;
    while(*end_ptr != '\0') {
        intSequence[number_of_elements] = (int)strtol(string, &end_ptr, 10);
        while(  *end_ptr == ' '  ||
                *end_ptr == '\n' ||
                *end_ptr == '\t') {
            end_ptr++;
        }
        string = end_ptr;
        number_of_elements++;
    }
    return number_of_elements;
}

void sort(int list[], int size) {
    int iterations = 0;
    int totalIterations = size - 1;
    _Bool unsorted = true;
    int temp;
    while(unsorted) {
        usleep(500 * 1000);
        unsorted = false;
        for(int i = 1; i < size; i++) {
            if(list[i - 1] > list[i]) {
                temp = list[i - 1];
                list[i - 1] = list[i];
                list[i] = temp;
                unsorted = true;
            }
        }
        printf("\rSorting progress: %d%%", (100 * (iterations) / totalIterations));
        fflush(stdout);
        iterations++;
    }
    printf("\rSorting progress: 100%%\n");
    printf("Sorted numbers:");
    for(int i = 0; i < size; i++) {
        printf(" %d", list[i]);
    }
    printf("\n");
}

void processTask(messageT* message) {
    int buffer[PIPE_BUF];

    printf("\nScheduling task with priority %d:\n", message->priority);
    size_t size_numbers = stringToInt(message->buffer, buffer);
    int numbers[size_numbers];
    for(size_t i = 0; i < size_numbers; i++) {
        numbers[i] = buffer[i];
    }
    sort(numbers, (int)size_numbers);
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: ./scheduler \"/csXXXX\"");
        return EXIT_FAILURE;
    }

    if(signal(SIGINT, signalHandler) == SIG_ERR) {
        printf("Signal registration failed!\n");
    }
    mq_unlink("/csbb4496");
    char* message_queue = argv[1];
    struct mq_attr mqAttr;
    mqAttr.mq_msgsize = PIPE_BUF;
    mqAttr.mq_maxmsg = MAX_MESSAGES;
    mqAttr.mq_flags = 0;
    mqAttr.mq_curmsgs = 0;
    mqd_t msq = mq_open(message_queue, O_RDONLY | O_CREAT, 0666, &mqAttr);
    if(msq == -1) {
        perror("mq open");
        return EXIT_FAILURE;
    }

    ssize_t bytes_read;
    messageT message;
    while(runner) {
        usleep(500 * 1000);
        mq_getattr(msq, &mqAttr);
        if(mqAttr.mq_curmsgs) {
            bytes_read = mq_receive(msq, message.buffer,
                                    PIPE_BUF, &message.priority);
            if(bytes_read == -1) {
                perror("mq receive");
                return EXIT_FAILURE;
            }
            message.buffer[bytes_read - 1] = '\0';

            processTask(&message);
        }
    }
    mq_getattr(msq, &mqAttr);
    while(mqAttr.mq_curmsgs) {
        mq_receive(msq, message.buffer, PIPE_BUF, NULL);
        mq_getattr(msq, &mqAttr);
    }
    mq_close(msq);
    mq_unlink(message_queue);
    return EXIT_SUCCESS;
}