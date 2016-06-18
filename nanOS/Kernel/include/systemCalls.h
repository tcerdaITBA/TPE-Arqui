#ifndef __SYSCALLS__H__
#define __SYSCALLS__H__

#define STDOUT 0
#define STDIN 1
#define STDERR 2

#include <stdint.h>

int sys_write(unsigned int fds, const char * str, unsigned int len);
int sys_read(unsigned int fds, char * str, unsigned int len);
int sys_time(int selection);
int sys_paint(int color, int x, int y);
int sys_wait(uint64_t milliseconds);
int sys_screen_res(uint64_t selection);
int sys_text_space(uint64_t selection);

#endif
