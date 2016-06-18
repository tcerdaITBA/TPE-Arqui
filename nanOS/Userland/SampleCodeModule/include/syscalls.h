#ifndef SYSCALLS_H
#define SYSCALLS_H

#define STDOUT 0
#define STDIN 1
#define STDERR 2
#include <stdint.h>

int write(unsigned int fd, void *buffer, unsigned int bytes);
int read(unsigned int fd, void *buffer, unsigned int bytes);
int time(int selector);
int paint(int color, int x, int y);
int wait(uint64_t milliseconds);
int screen_Xresolution();
int screen_Yresolution();

#endif
