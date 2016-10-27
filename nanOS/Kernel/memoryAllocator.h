#ifndef _MEM_ALLOCATOR_
#define _MEM_ALLOCATOR_

#include <stdint.h>

void initialize_memory_allocator();
uint64_t get_page(uint64_t size);
uint64_t store_page(uint64_t address);


#endif