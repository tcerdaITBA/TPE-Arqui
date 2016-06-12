#include <stdint.h>
#include <stdarg.h>
#include "stdio.h"
#include "strings.h"

#define SIZE 256

void putchar(int c) {
	write(STDOUT, &c, 1);
}

int getchar() {
    int c;
    read(STDIN, &c, 1);
    putchar(c);
    return c;
}

static int prints(const char *str) {
	int i;
	for (i=0; str[i] != '\0'; i++)
		putchar(str[i]);
	return i;
}

static int printi(int64_t value) {
	char buffer[SIZE]; // buffer para guardar el resultado de itoa
	itoa(value, buffer, 10); // guarda en buffer el string del valor en base 10
	return prints(buffer);
}

static int printb(int value) {
	char buffer[SIZE]; // buffer para guardar el resultado de itoa
	itoa(value, buffer, 2); // guarda en buffer el string del valor en base 2
	return prints(buffer);
}

static int printx(int value) {
	char buffer[SIZE]; // buffer para guardar el resultado de itoa
	itoa(value, buffer, 16); // guarda en buffer el string del valor en base 16
	return prints(buffer);
}


/* Imprime a pantalla con el formato indicado. Solo soporta los siguientes
** simbolos: d,i,s,x,b.
** Retorna la cantidad de caracteres escritos.
** En caso de simbolo invalido imprime todos los caracteres hasta la ocurrencia
** de dicho simbolo donde deja de imprimir y retorna.
*/
int printf(const char *format, ...) {
	int len = 0; // cantidad de caracteres escritos
	va_list args;

	va_start(args, format);

	while(*format != '\0') {
		char c = *format++;
		char symbol;
		if(c != '%') {
			putchar(c);
			len++;
		} 
		else {
			symbol = *format++;
			switch(symbol) {
				case 'd': 
				case 'i':
					len += printi(va_arg(args, int64_t));
					break;
				case 's':
					len += prints(va_arg(args, char *));
					break;
				case 'x':
					len += printx(va_arg(args, int));
					break;
				case 'b':
					len += printb(va_arg(args, int));
					break;
				case '%':
					putchar('%');
					len++;
					break;
				default:  // simbolo invalido. Termina la funcion y retorna
					return len;
			}
		}
	}
	return len;
}