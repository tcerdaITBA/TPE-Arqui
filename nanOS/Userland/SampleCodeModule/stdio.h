#ifndef STDIO_H
#define STDIO_H

#include "syscalls.h"

int getchar();
void putchar(int c);
int printf(const char *format, ...);
int readline(char *str, int maxlen);

#endif
