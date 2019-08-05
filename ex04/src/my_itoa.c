#include "../include/test.h"
#include "stdio.h"
#include "stdlib.h"

int my_abs(double d) {
    if (d < 0)
        d = -d;

    return d;
}

char* my_itoa(int nmb) {
    int i = 1;
    static char buf[sizeof(int)+2];
    char *res = buf + sizeof(int) + 1;

    do {
        *--res = '0' + (my_abs(nmb/i)%10);
        i *= 10;
    } while (nmb/i);

    if (nmb < 0)
        *--res = '-';

    return res;
}
