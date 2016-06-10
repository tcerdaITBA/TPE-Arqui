#include "stdio.h"

void putchar(int c) {
	write(STDOUT, &c, 1);
}

int getchar() {
    int c;
    read(STDIN, &c, 1);
    putchar(c);
    return c;
}