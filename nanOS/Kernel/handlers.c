#include <stdint.h>
#include "videoDriver.h"
#include "keyboardDriver.h"
#include "handlers.h"
#include "systemCalls.h"

static int i = 0;

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
	}
	return;

}

void int_20() {
	if (i % 90 == 0) // pasaron 5 segundos aprox
		print_num(i,0,0);
	i++;
}

void int_21() {
	store_scancode();
}

uint64_t syscallDispatcher(uint64_t rax, uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	switch (rax) {
		case 3:
			return sys_read(rbx, (char *) rcx, rdx);
		case 4:
			return sys_write(rbx, (const char *) rcx, rdx);
		case 5:
			return sys_time(rbx);
		case 6:
			return sys_paint(rbx, rdx, rcx);
	}
	return -1;
}
