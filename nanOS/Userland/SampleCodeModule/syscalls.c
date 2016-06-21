#include <stdint.h>
#include "syscalls.h"
#include "stdio.h"

/* Contiene todas las llamadas a int80 */
uint64_t _int80h(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx);

/* SystemCall Read */
int read(unsigned int fd, void *buffer, unsigned int bytes) {
    return _int80h(3, fd, (uint64_t) buffer, bytes);
}

/* SystemCall Write */
int write(unsigned int fd, void *buffer, unsigned int bytes) {
    return _int80h(4, fd, (uint64_t) buffer, bytes);
}

/* SystemCall Time */
int time(int selector) {
    return _int80h(5, selector, 0, 0);
}

/* SystemCall Paint */
int paint(int color, int x, int y) {
    return _int80h(6, color, x, y);
}

/* SystemCall Wait */
int wait(unsigned long int milliseconds) {
	return _int80h(7, milliseconds, 0, 0);
}

/* SystemCall para obtener la resolucion de pantalla */
int screen_Xresolution() {
    return _int80h(8, 0, 0, 0);
}

/* SystemCall para obtener la resolucion de pantalla */
int screen_Yresolution() {
    return _int80h(8, 1, 0, 0);
}

/* SystemCall para obtener las filas*/
int text_rows() {
    return _int80h(9, 0, 0, 0);
}

/* SystemCall para obtener las columnas */
int text_cols() {
    return _int80h(9, 1, 0, 0);
}

/* SystemCall para reservar espacio en memoria*/
void * reserve (unsigned int bytes) {
	return (void *) _int80h(10, bytes, 0, 0);
}

/* SystemCall para obtener la direccion de memoria de datos */
void * data_address() {
    return (void *) _int80h(11, 0, 0, 0);
}

int set_char_color (uint64_t r, uint64_t g, uint64_t b) {
    return _int80h(12,r,g,b);
}

int set_bg_color (uint64_t r, uint64_t g, uint64_t b) {
    return _int80h(13,r,g,b);
}