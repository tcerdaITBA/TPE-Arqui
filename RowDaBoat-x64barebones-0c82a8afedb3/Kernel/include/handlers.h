#ifndef HANDLERS_H_
#define HANDLERS_H_

#include <stdint.h>

void irqDispatcher(uint32_t irq);

void int_08_hand();

#endif /* HANDLERS_H_ */
