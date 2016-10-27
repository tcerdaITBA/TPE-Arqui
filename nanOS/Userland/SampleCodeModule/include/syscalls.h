#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define STDOUT 0
#define STDIN 1
#define STDERR 2

int write(unsigned int fd, void *buffer, unsigned int bytes);
int read(unsigned int fd, void *buffer, unsigned int bytes);
int time(int selector);
int paint(int color, int x, int y);
int wait(uint64_t milliseconds);
int screen_Xresolution();
int screen_Yresolution();
int text_rows();
int text_cols();
void * reserve (unsigned int bytes);
int free_reserve(void * address);
void * data_address();
int set_char_color (uint64_t r, uint64_t g, uint64_t b);
int set_bg_color (uint64_t r, uint64_t g, uint64_t b);

#endif
