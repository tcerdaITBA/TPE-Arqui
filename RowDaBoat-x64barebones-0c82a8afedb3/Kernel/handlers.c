#include <stdint.h>
#include "videoDriver.h"
#include "keyboardDriver.h"
#include "handlers.h"
#include "systemCalls.h"

static int i = 0;

static void int_08();
static void int_09();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_08();
			break;
		case 1:
			int_09();
			break;
	}
	return;

}

void int_08() {
	if (i % 90 == 0) // pasaron 5 segundos aprox
		print_num(i,0,0);
	i++;
}

void int_09() {
	int c = get_char();
	if (c != -1)
		put_char(c);
}

void syscallDispatcher(uint64_t rax, uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	switch (rax) {
		case(4):
			sys_write(rbx, (const char *) rcx, rdx);
			break;
	}
}
