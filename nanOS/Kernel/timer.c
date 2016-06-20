#include "timer.h"
#include "interrupts.h"

static uint64_t ticks = 0;

/* Controla la cantidad de ticks del timer tick */
void timer_handler() {
	ticks++;
}

/* Pone en reposo la pantalla por una cantidad de milisegundos recibida por parámetro,
** contando la cantidad de "ticks"(interrupciones) del timer Tick */
int sleep(uint64_t milliseconds) {
	_cli();
	uint64_t wait_ticks = milliseconds / 55; // Hay 1 tick cada 55 milisegundos aproximadamente
	ticks = 0;
	_sti();
	while(ticks < wait_ticks)
		_hlt();
	return ticks < wait_ticks;
}