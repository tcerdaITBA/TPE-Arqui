#include "include/handlers.h"

static int i = 0;
static char* video = 0xB8000;

static void int_08();
static void int_09();

void irqDispatcher(uint32_t irq) {
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
	video[i++] = i;
}

void int_09() {
	return;
}
