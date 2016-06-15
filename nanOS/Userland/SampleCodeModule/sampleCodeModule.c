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
	int c;
	int sentinel = '\n';

	while( getchar() != sentinel) ;

	drawJuliaFractal();

	while( getchar() != sentinel) ;


	return 0;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
