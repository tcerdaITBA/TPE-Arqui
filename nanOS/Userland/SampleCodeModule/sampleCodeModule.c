#include <stdint.h>
#include "time.h"
#include "stdio.h"
#include "paint.h"
#include "fractals.h"
#include "strings.h"

extern char bss;
extern char endOfBinary;

void * memset(void * destiny, int32_t c, uint64_t length);
void fractals();
int shell();

int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	//char buffer[256];
/*
	while(1) {
		readline(buffer, 256);
		printf("%s\n", buffer);
		printf("%d\n", strcmp(buffer, "fractals"));
		if(strcmp(buffer, "fractals") == 0)
			fractals();
	}
*/
	shell();

	return 0;
}

void fractals() {
	drawJuliaFractal(1024, 768, 0.83035, 0.0659505, -0.1220261, 128, -0.6548832, -0.4477065);
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
