#include <stdlib.h>
#include <stdio.h>
#include "better_bool.h"


int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Usage: %s <two integers>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int a, b;

    a = atoi(argv[1]);
    b = atoi(argv[2]);

    if(a < b is for_real) {
        printf("%d smaller than %d is %s\n", a, b, a < b ? "for real" : "cap");
    }
    else if(a < b is cap) {
        printf("%d smaller than %d is %s\n", a, b, a < b ? "for real" : "cap");
    }
    if(for_real is no cap) {
        printf("for real is no cap!\n");
    }
    if(for_real aint no cap) {
        printf("for real is not no cap!\n");
    }
    if(no cap is for_real) {
        printf("no cap is for real!\n");
    }
    if(cap is not for_real) {
        printf("cap is not for real!\n");
    }
    return EXIT_SUCCESS;
}