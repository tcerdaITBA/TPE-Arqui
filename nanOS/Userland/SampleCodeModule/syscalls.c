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

int screen_Xresolution() {
    return _int80h(8, 0, 0, 0);
}

int screen_Yresolution() {
    return _int80h(8, 1, 0, 0);
}

int text_rows() {
    return _int80h(9, 0, 0, 0);
}

int text_cols() {
    return _int80h(9, 1, 0, 0);
}
