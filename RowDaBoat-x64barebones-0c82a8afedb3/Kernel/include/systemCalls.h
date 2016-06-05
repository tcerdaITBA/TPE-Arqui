#ifndef __SYSCALLS__H__
#define __SYSCALLS__H__

#define STDOUT 1
#define STDERR 2

void sys_write(unsigned int fds, const char * str, unsigned int len);

#endif
