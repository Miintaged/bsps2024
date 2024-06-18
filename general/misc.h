#ifndef _MISC_H
#define _MISC_H

#include <stdlib.h>

int parse_int(const char* str, int* value) {
    char* ptr;
    *value = strtol(str, &ptr, 10);
    return *ptr == '\0';
}

int parse_double(const char* str, double* value) {
    char* ptr;
    *value = strtod(str, &ptr);
    return *ptr == '\0';
}

void null_terminate(char message[], size_t size) {
    if (message[size - 1] == '\n') {
        message[size - 1] = '\0';
    } else {
        message[size] = '\0';
    }
}
#endif
