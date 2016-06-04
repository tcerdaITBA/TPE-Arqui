#include "strings.h"

int str_len(char *str) {
	int i;
	for (i = 0; str[i] != '\0'; i++)
		;
	return i;
}
