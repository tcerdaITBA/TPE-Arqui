#ifndef __SYSCALLS__H__
#define __SYSCALLS__H__

#define STDOUT 0
#define STDIN 1
#define STDERR 2

int sys_write(unsigned int fds, const char * str, unsigned int len);
int sys_read(unsigned int fds, char * str, unsigned int len);
int sys_time(int selection);

#endif
