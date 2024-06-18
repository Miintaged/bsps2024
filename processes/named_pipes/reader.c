#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>
#include "fifo_functions.h"

#define MAX_CLIENTS 5
#define FIFO_PATH "/tmp/csbb4496_"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./server [<client-name>]\n");
        exit(EXIT_FAILURE);
    }
    int counter                 = 0;
    int clients                 = argc - 1;
    int active_clients          = clients;
    int fifo_fd[MAX_CLIENTS];

    for (int i = 0; i < clients; ++i) {
        char fifo_path[50];
        snprintf(fifo_path, sizeof(fifo_path), "%s%s", FIFO_PATH, argv[i + 1]);
        mkfifo_safe(fifo_path, 0666);
        fifo_fd[i] = open_safe(fifo_path, O_RDONLY);
        printf("%s connected.\n", argv[i + 1]);
    }

    struct pollfd fds[MAX_CLIENTS];
    for (int i = 0; i < clients; ++i) {
        fds[i].fd = fifo_fd[i];
        fds[i].events = POLLIN;
    }

    while (active_clients > 0) {
        poll_safe(fds, clients, -1);
        for (int i = 0; i < clients; ++i) {
            if (fds[i].revents & POLLIN) {
                char buf[256];
                ssize_t bytes_read = read(fds[i].fd, buf, 256);
                if (bytes_read == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }

                else if (bytes_read > 0) {
                    buf[bytes_read] = '\0';

                    int num;
                    if (sscanf(buf, "%d", &num) == 1) {
                        counter += num;
                        printf("%s: counter = %d.\n", argv[i + 1], counter);
                    }
                    else {
                        printf("%s: %s is malformed.\n", argv[i + 1], buf);
                    }
                }
            }
            else if(fds[i].revents & POLLHUP) {
                printf("%s disconnected.\n", argv[i + 1]);
                close(fds[i].fd);
                active_clients--;
            }
        }
    }
    close_all(fifo_fd, clients);
    for (int i = 0; i < clients; ++i) {
        char fifo_path[50];
        snprintf(fifo_path, sizeof(fifo_path), "%s%s", FIFO_PATH, argv[i + 1]);
        if (unlink(fifo_path) == -1) {
            perror("unlink");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}