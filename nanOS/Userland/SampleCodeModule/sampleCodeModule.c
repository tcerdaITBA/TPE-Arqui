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

	int h = hour();
	int m = minutes();
	int s = seconds();


	printf("   %d:%d:%d  ",h,m,s );

	while( getchar() != sentinel) ;

	drawJuliaFractal(1024, 768, 0.83035, 0.0659505, -0.1220261, 128, -0.6548832, -0.4477065);

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
