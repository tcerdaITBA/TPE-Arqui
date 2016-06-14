#include <stdint.h>
#include "time.h"
#include "stdio.h"

extern char bss;
extern char endOfBinary;

void * memset(void * destiny, int32_t c, uint64_t length);

int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	printf("               ");
	int min = minutes();
	int sec = seconds();
	int h = hour();
	printf(" la hora %d:%d:%d\n ", hour(), minutes(), seconds());
	printf(" LA HORA %d:%d:%d\n", h,min,sec);

	while(getchar()!= 'p');

	return 0;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
