#include <stdint.h>
#include "syscalls.h"

int _int80h(uint64_t rax, uint64_t rbx, uint64_t rcx,uint64_t rdx);

int read(unsigned int fd, void *buffer, unsigned int bytes) {
    return _int80h(3, fd, (uint64_t) buffer, bytes);
}

int write(unsigned int fd, void *buffer, unsigned int bytes) {
    return _int80h(4, fd, (uint64_t) buffer, bytes);
}

int time(int selector) {
    return _int80h(5, selector, 0, 0);
}

int paint(int color, int x, int y) {
    return _int80h(6, color, x, y);
}

int wait(unsigned long int milliseconds) {
	return _int80h(7, milliseconds, 0, 0);
}