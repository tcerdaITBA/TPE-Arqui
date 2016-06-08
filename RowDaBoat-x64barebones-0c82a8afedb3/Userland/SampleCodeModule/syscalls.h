#ifndef SYSCALLS_H
#define SYSCALLS_H 

#define STDOUT 0
#define STDIN 1
#define STDERR 2

int write(unsigned int fd, void *buffer, unsigned int bytes);
int read(unsigned int fd, void *buffer, unsigned int bytes);


#endif
