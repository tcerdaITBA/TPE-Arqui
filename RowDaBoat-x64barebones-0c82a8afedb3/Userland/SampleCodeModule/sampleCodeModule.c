#include <stdint.h>


extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

void * memset(void * destiny, int32_t c, uint64_t length);
void write(unsigned int fd, void * buffer, unsigned int bytes);

int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	char str[] = "HOLA DESDE USERLAND";
	int len = sizeof(str);

	write(1,str,len);

	return 0;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
