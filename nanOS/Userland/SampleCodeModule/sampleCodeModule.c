#include <stdint.h>
#include "time.h"
#include "stdio.h"
#include "paint.h"

extern char bss;
extern char endOfBinary;

void * memset(void * destiny, int32_t c, uint64_t length);

int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	int i;
	int j;
	for (j=0; j<16; j++) {
		for (i=0; i < 1000; i++) {
			paint_pixel(255,0,200,i,j);
		}
	}

	while(getchar() != 'p');

	return 0;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
