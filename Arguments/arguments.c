#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define ARGUMENT_COUNT = 5;

long double validate_argument_long_double(char const* str) {
    errno = 0;
    char *endptr;
    long double result = strtold(str, &endptr);

    if (errno == ERANGE) {
        fprintf(stderr, "Range error occurred, value too large or too small\n");
        exit(EXIT_FAILURE);
    }
    if (endptr == str) {
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }
    if (*endptr != '\0') {
        fprintf(stderr, "Non-numeric characters found: %s\n", endptr);
        exit(EXIT_FAILURE);
    }

    return result;
}

unsigned long long validate_argument_unsigned_long_long(char const* str) {
    errno = 0;
    char *endptr;
    long long result = strtoull(str, &endptr, 10);

    if (errno == ERANGE) {
        if (result == ULLONG_MAX) {
            fprintf(stderr, "Overflow occurred, value too large\n");
        } else {
            fprintf(stderr, "Range error occurred\n");
        }
        exit(EXIT_FAILURE);
    }

    if (endptr == str) {
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

    if (*endptr != '\0') {
        fprintf(stderr, "Non-numeric characters found: %s\n", endptr);
        exit(EXIT_FAILURE);
    }

    return result;
}

long long validate_argument_long_long(char const* str) {
    errno = 0;
    char *endptr;
    long long result = strtoll(str, &endptr, 10);

    if (errno == ERANGE) {
        if (result == LLONG_MAX) {
            fprintf(stderr, "Overflow occurred, value too large\n");
        } else if (result == LLONG_MIN) {
            fprintf(stderr, "Underflow occurred, value too small\n");
        } else {
            fprintf(stderr, "Range error occurred\n");
        }
        exit(EXIT_FAILURE);
    }

    if (endptr == str) {
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

    if (*endptr != '\0') {
        fprintf(stderr, "Non-numeric characters found: %s\n", endptr);
        exit(EXIT_FAILURE);
    }

    return result;
}

// EXAMPLE USAGE
// int main(int argc, char const *argv[]) {
//     int n = validate_argument_long_long(argv[0]);
//     return EXIT_SUCCESS;
// }
