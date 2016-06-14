#ifndef __SYSCALLS__H__
#define __SYSCALLS__H__

#define STDOUT 0
#define STDIN 1
#define STDERR 2

void sys_write(unsigned int fds, const char * str, unsigned int len);
void sys_read(unsigned int fds, char * str, unsigned int len);

#endif
