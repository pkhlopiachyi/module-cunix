#include "../include/test.h"
#include "stdio.h"

int my_atoi(const char *nptr) {
    long int i=0;
    int flag = 0;

    if (*nptr == '-') {
        nptr++;
        flag = 1;
    }

    while (*nptr != '\0') {
        if (*nptr >= '0' && *nptr <='9') {
            i *= 10;
            i += *nptr - '0';
        } else {
            break;
        }
        nptr++;
    }

    if (flag == 1) {
        i = -i;
    }
    
    return i;
}
