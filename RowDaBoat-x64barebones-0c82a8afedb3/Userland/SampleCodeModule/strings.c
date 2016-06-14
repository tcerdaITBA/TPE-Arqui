#include "strings.h"

#define UINT_MAX 4294967295 //TODO: limits.h

int str_len(char *str) {
	int i;
	for (i = 0; str[i] != '\0'; i++)
		;
	return i;
}

int strcmp(char * s, char * t) {
	return strncmp(s, t, UINT_MAX);
}

int strncmp(char * s, char * t, unsigned int n) {
	int i;
	for (i = 0; i < n && s[i] != '\0' && t[i] != '\0' && s[i] == t[i]; i++);
	return s[i]-t[i];
}

// devuelve la cantidad de caracteres copiados
int strcpyto(char * to, char * from, char limit) {
	return strcpynto(to, from, limit, UINT_MAX);
}

int strcpynto(char * to, char * from, char limit, unsigned int n) {
	int i;
	for (i = 0; i < n && from[i] != limit; i++) {
		to[i] = from[i];
	}
	return i;
}

// Copia en str los valores ascii de los digitos de value en la base indicada.
// Devuelve la cantidad de digitos copiados.
int itoa(int value, char *str, int base) {
	char *p = str;
	char *p1, *p2;
	int len = 0;

	if(value < 0 && base == 10) {
		value = -value;
		len++;
		*p++ = '-';
		str++;
	}

	//Calculate characters for each digit
	do
	{
		int remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		len++;
	}
	while (value /= base);

	// Terminate string in str.
	*p = '\0';

	//Reverse string in str.
	p1 = str;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return len;
}
