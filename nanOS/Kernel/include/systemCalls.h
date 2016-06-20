#ifndef __SYSCALLS__H__
#define __SYSCALLS__H__

#define STDOUT 0
#define STDIN 1
#define STDERR 2

#include <stdint.h>

uint64_t sys_write(uint64_t fds, const char * str, uint64_t len);
uint64_t sys_read(uint64_t fds, char * str, uint64_t len);
uint64_t sys_time(uint64_t selection);
uint64_t sys_paint(uint64_t color, uint64_t x, uint64_t y);
uint64_t sys_wait(uint64_t milliseconds);
uint64_t sys_screen_res(uint64_t selection);
uint64_t sys_text_space(uint64_t selection);
uint64_t sys_malloc(uint64_t bytes);
uint64_t sys_data_address();

#endif
