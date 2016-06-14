#include <stdint.h>
#include "stdio.h"

extern char bss;
extern char endOfBinary;

void * memset(void * destiny, int32_t c, uint64_t length);

int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	char str[] = "HOLA DESDE USERLAND";
	char str2[] = "CHAU DESDE USERLAND";

	for (int i = 0; str[i] != '\0'; i++)
		putchar(str[i]);

	printf(" se escriberon %d caracteres detras mio", printf("hola soy printf %d",-50));
	printf("%s y %s%c? Mejor solo %d %s", str, str2, '?', 1, "saludo\n");
	while(getchar()!='\n');
	
	for (int i = 0; str2[i] != '\0'; i++)
		putchar(str2[i]);

	return 0;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
