#include "keyboardDriver.h"

int get_key();

/* los que tiene '\0' representan cosas que no se imprimen, como ESC */
static const char map[] = { '\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', '¿', '\b',
						'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '\0', '+', '\n',
						'\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'ñ', '{', '|', '\0',
						'}', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', '\0', '\0', '\0', ' '
					};

static int len = sizeof(map);

int get_char() {
	int k;
	if ((k = get_key()) > 0 && k < len && map[k] != '\0')
		return map[k];
	return -1;
}
