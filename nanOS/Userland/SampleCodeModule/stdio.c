#include <stdint.h>
#include <stdarg.h>
#include "stdio.h"
#include "strings.h"
#include "ctype.h"

#define BUFFER_SIZE 1024
#define AUX_SIZE 64

#define SPACE 1

static unsigned char buffer[BUFFER_SIZE];
static int buffi = 0;
static void fill_buffer();

void putchar(int c) {
	write(STDOUT, &c, 1);
}

/*
** Lee del buffer hasta '\n' caso en el cual lo marca como vacio.
** Si esta vacio el buffer lo llena.
*/
int getchar() {
	if (buffi == 0) // buffer vacio
		fill_buffer();
    unsigned char c = buffer[buffi++]; // Se lee caracter del buffer
    if (c == '\n' || buffi == BUFFER_SIZE)
    	buffi = 0;  // reset buffer
    return c;
}

int readline(char *str, unsigned int maxlen) {
    unsigned int i;
    int c;
    for (i = 0; i < maxlen-1 && (c = getchar()) != '\n'; i++) 
			str[i] = c;
    str[i] = '\0';
    return i;
}

/*
** Igual que readline pero no se borran los espacios iniciales y finales 
** de la cadena como tambien los espacios repetidos.
*/
int readline_no_spaces(char *str, unsigned int maxlen) {
	unsigned int i = 0;
	int c;
	char state = SPACE;
	while ((c = getchar()) != '\n' && i < maxlen-1) {
		if (state != SPACE) {
			str[i++] = c;
			if (isspace(c))
				state = SPACE;
		}
		else if (!isspace(c)) {
			str[i++] = c;
			state = !SPACE;
		}
	}
	if (i > 0 && isspace(str[i-1]))
		i--;  // Se borra el utimo espacio si lo hay
	str[i] = '\0';
	return i;
}

static int prints(const char *str) {
	int i;
	for (i=0; str[i] != '\0'; i++)
		putchar(str[i]);
	return i;
}

static int printi(int value, char aux[]) {
	itoa(value, aux, 10); // guarda en buffer el string del valor en base 10
	return prints(aux);
}

static int printb(int value, char aux[]) {
	itoa(value, aux, 2); // guarda en buffer el string del valor en base 2
	return prints(aux);
}

static int printx(int value, char aux[]) {
	itoa(value, aux, 16); // guarda en buffer el string del valor en base 16
	return prints(aux);
}


/* Imprime a pantalla con el formato indicado. Solo soporta los siguientes
** simbolos: d,i,s,x,b,c.
** Retorna la cantidad de caracteres escritos.
** En caso de simbolo invalido imprime todos los caracteres hasta la ocurrencia
** de dicho simbolo donde deja de imprimir y retorna.
*/
int printf(const char *format, ...) {
	int len = 0; // cantidad de caracteres escritos
	char aux[AUX_SIZE];
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
					len += printi(va_arg(args, int), aux);
					break;
				case 's':
					len += prints(va_arg(args, char *));
					break;
				case 'x':
					len += printx(va_arg(args, int), aux);
					break;
				case 'b':
					len += printb(va_arg(args, int), aux);
					break;
				case 'c':
					putchar(va_arg(args,int) & 0xFF);
					len++;
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
	va_end(args);
	return len;
}

static void fill_buffer() { // llena el buffer hasta '\n' o se supero su capacidad
	unsigned char c; 
	int i = 0;
	do {
		read(STDIN, &c, 1);

		if (c == '\b' && i > 0) { // si hay algo para borrar borra del buffer y la pantalla
			i--;
			if (buffer[i] != '\t')
				putchar(c);  // Se borra el caracter de la pantalla si no era un tab
		}

		else if (c != '\b') {
			buffer[i++] = c;
			putchar(c);   // Se muestra el caracter en pantalla
		}

	} while (c != '\n' && i < BUFFER_SIZE-1);
}