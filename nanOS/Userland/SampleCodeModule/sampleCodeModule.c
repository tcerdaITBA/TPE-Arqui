#include <stdint.h>
#include "stdlib.h"

extern char bss;
extern char endOfBinary;

int shell();

int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	shell();

	return 0;
}