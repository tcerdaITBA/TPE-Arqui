#include <stdint.h>
#include "videoDriver.h"
#include "keyboardDriver.h"
#include "handlers.h"

static int i = 0;

static void int_08();
static void int_09();

void irqDispatcher(int irq) {
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
	int c = get_key();
	print_num(i++,0,0);
}

void int_09() {
	int c = get_key();
	print_num(c,3,3);
}
