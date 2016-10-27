#include "dirs.h"
#include "memoryAllocator.h"


#define STACK_SIZE ((page_index_t) -1) /* valor máximo que puede tomar una variable de tipo page_index_t */


typedef uint16_t page_index_t;

static page_index_t pages_stack[ STACK_SIZE ];
static page_index_t current_index = 0; /* Apunta al elemento a poppear */

/* Se inicializan los indices en el stack */
/* Comienza guardando en 1 porque al comienzo de data están los parametros de los fractales */
void initialize_memory_allocator() {
	int i;
	for (i = 0; i < STACK_SIZE-1; i++)
		pages_stack[i] = i+1;
}

/* Se ignora el valor de size. Se asume que será menor a PAGE_SIZE.
** Se hizo así para que la interfaz tenga sentido, en un futuro puede
** cambiar la implementación a una que si contemple el size correctamente.
*/
uint64_t get_page(uint64_t size) {
	if (current_index < STACK_SIZE)
		return pages_stack[current_index++] * PAGE_SIZE + DATA_ADDRESS;
	return 0;
}

uint64_t store_page(uint64_t address) {
	if (current_index > 0) {
		pages_stack[--current_index] = (address - DATA_ADDRESS) / PAGE_SIZE;
		return 1;
	}
	return 0;
}