#include "common.h"

#define FIFO_PATH "/tmp/csbb4496_"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <client-name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char fifo_path[50];
    snprintf(fifo_path, sizeof(fifo_path), "%s%s", FIFO_PATH, argv[1]);

    int fifo_fd = open(fifo_path, O_WRONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }


    while (1) {
        printf("Expression: ");
        char expression[256];
        if (fgets(expression, sizeof(expression), stdin) == NULL) {
            break;
        }

        size_t len = strlen(expression);
        if (len > 0 && expression[len - 1] == '\n') {
            expression[len - 1] = '\0';
        }

        if (strlen(expression) == 0) {
            break;
        }

        ssize_t bytes_written = write(fifo_fd, expression, strlen(expression));
        if (bytes_written == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    close(fifo_fd);
    return 0;
}