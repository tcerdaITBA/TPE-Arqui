#include "videoDriver.h"
#include "keyboardDriver.h"
#include "systemCalls.h"
#include "interrupts.h"
#include "rtc.h"
#include "timer.h"
#include "lib.h"
#include "dirs.h"

#define ERR_COLOR 255,0,0  // Rojo

#define SYS_SIZE (sizeof(syscalls)/sizeof(syscalls[0]))

static uint64_t sys_write_wr(uint64_t fds, uint64_t str, uint64_t length);
static uint64_t sys_read_wr(uint64_t fds, uint64_t buffer, uint64_t len);
static uint64_t sys_time_wr(uint64_t selection, uint64_t unused1, uint64_t unused2);
static uint64_t sys_wait_wr(uint64_t milliseconds, uint64_t unused1, uint64_t unused2);
static uint64_t sys_screen_res_wr(uint64_t selection, uint64_t unused1, uint64_t unused2);
static uint64_t sys_text_space_wr(uint64_t selection, uint64_t unused1, uint64_t unused2);
static uint64_t sys_malloc_wr(uint64_t bytes, uint64_t unused1, uint64_t unused2);
static uint64_t sys_data_address_wr(uint64_t unused1, uint64_t unused2, uint64_t unused3);

/* Vector de system calls */
static uint64_t (*syscalls[]) (uint64_t,uint64_t,uint64_t) = { 0,0,0, 		/* 0, 1, 2 system calls reservados*/
															   sys_read_wr,         /* 3 */
															   sys_write_wr,        /* 4 */
															   sys_time_wr,         /* 5 */
															   sys_paint,           /* 6 */
															   sys_wait_wr,         /* 7 */
															   sys_screen_res_wr,   /* 8 */
															   sys_text_space_wr,   /* 9 */
															   sys_malloc_wr,       /* 10 */
															   sys_data_address_wr, /* 11 */
															   sys_set_char_color,  /* 12 */
															   sys_set_bg_color     /* 13 */
															};

/* Ejecuta la system call correspondiente al valor de rax */
uint64_t syscallDispatcher(uint64_t rax, uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	if (rax < SYS_SIZE && rax >= 3)
		return (*syscalls[rax])(rbx,rcx,rdx);
	return 0;
}

/* SystemCall de Read para leer de entrada estándar*/
uint64_t sys_read(uint64_t fds, char * buffer, uint64_t bytes) {
	unsigned int i = 0;
	char c;
    if (fds == STDIN) {
			while (i < bytes) {
				c = get_char();
				if (c != -1) {
					buffer[i++] = c;
				} else {
					_hlt();
				}
			}
    }
    return i;
}

/* SystemCall de Write para escribir a salida estándar */
uint64_t sys_write(uint64_t fds, const char * str, uint64_t length) {
	if (fds == STDERR) {
		unsigned char r,g,b; // Backup
		current_char_color(&r,&g,&b);
		set_char_color(ERR_COLOR);
		put(str, length);
		set_char_color(r,g,b);
	} 
	else if (fds == STDOUT) {
		put(str, length);
		return length;
	}
	return 0;
}

/* SystemCall de Time, retorna hora, minutos y segundos actuales */
uint64_t sys_time(uint64_t selection) {
    switch(selection) {
	case 0:
	    return seconds();
	case 1:
	    return minutes();
	case 2:
	    return hour();
    }
    return -1;
}

/* SystemCall de paint para pintar un pixel */
uint64_t sys_paint(uint64_t color, uint64_t x, uint64_t y) {
	char r = (color >> 16) & 0xFF;
	char g = (color >> 8) & 0xFF;
	char b = color & 0xFF;

	return fill(r,g,b,x,y); // 1 si fue un pixel válido, 0 sino.
}

/* SystemCall de wait, Pone en espera a la pantalla por la cantidad de milisegundos pasados por parámetro*/
uint64_t sys_wait(uint64_t milliseconds) {
	return sleep(milliseconds);
}

/* SystemCall que retorna la resolucion de la pantalla*/
uint64_t sys_screen_res(uint64_t selection) {
	switch (selection) {
		case 0: // screen width
			return scr_x_res();
		case 1: // screen height
			return scr_y_res();
	}
	return -1;
}

/* SystemCall que retorna la cantidad de filas o columnas de la pantalla */
uint64_t sys_text_space(uint64_t selection) {
	switch (selection) {
		case 0: // screen width
			return text_rows();
		case 1: // screen height
			return text_cols();
	}
	return -1;
}

/* SystemCall Malloc */
uint64_t sys_malloc(uint64_t bytes) {
	return (uint64_t) malloc(bytes);
}

/* System call que retorna la dirección del módulo de datos */
uint64_t sys_data_address() {
	return DATA_ADDRESS;
}

uint64_t sys_set_char_color(uint64_t r, uint64_t g, uint64_t b) {
	return set_char_color(r,g,b);
}

uint64_t sys_set_bg_color(uint64_t r, uint64_t g, uint64_t b) {
	return set_bg_color(r,g,b);
}

/* WRAPPERS */
/* Se usan para system calls que no reciben exactamente 3 parametros enteros.
** En la wrapper se filtran los parametros innecesarios y se hacen los casteos
** que hagan falta. De esta forma se puede tener un vector de system calls sin
** que la implementacion de ellas se vea afectada.
*/

static uint64_t sys_write_wr(uint64_t fds, uint64_t str, uint64_t length) {
	return sys_write(fds, (const char *) str, length);
}

static uint64_t sys_read_wr(uint64_t fds, uint64_t buffer, uint64_t bytes) {
	return sys_read(fds, (char *) buffer, bytes);
}

static uint64_t sys_time_wr(uint64_t selection, uint64_t unused1, uint64_t unused2) {
	return sys_time(selection);
}

static uint64_t sys_wait_wr(uint64_t selection, uint64_t unused1, uint64_t unused2) {
	return sys_wait(selection);
}

static uint64_t sys_screen_res_wr(uint64_t selection, uint64_t unused1, uint64_t unused2) {
	return sys_screen_res(selection);
}

static uint64_t sys_text_space_wr(uint64_t selection, uint64_t unused1, uint64_t unused2) {
	return sys_text_space(selection);
}

static uint64_t sys_malloc_wr(uint64_t selection, uint64_t unused1, uint64_t unused2) {
	return sys_malloc(selection);
}

static uint64_t sys_data_address_wr(uint64_t unused1, uint64_t unused2, uint64_t unused3) {
	return sys_data_address();
}