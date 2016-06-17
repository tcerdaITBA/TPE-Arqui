#include "strings.h"
#include "limits.h"
#include "ctype.h"

int str_len(const char *str) {
	int i;
	for (i = 0; str[i] != '\0'; i++)
		;
	return i;
}

int strcmp(const char * s, const char * t) {
	return strncmp(s, t, UINT_MAX);
}

int strncmp(const char * s, const char * t, unsigned int n) {
	int i;
	for (i = 0; i < n-1 && s[i] != '\0' && t[i] != '\0' && s[i] == t[i]; i++)
		;
	return s[i]-t[i];
}

// devuelve la cantidad de caracteres copiados
int strcpyto(char * to, const char * from, char limit) {
	return strcpynto(to, from, limit, UINT_MAX);
}

int strcpynto(char * to, const char * from, char limit, unsigned int n) {
	int i;
	for (i = 0; i < n && from[i] != limit && from[i] != '\0'; i++) {
		to[i] = from[i];
	}
	to[i] = '\0';
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

int atoi(const char *str) {
	while (isspace(*str))
		str++;

	int num = 0;
	int sign = 1;
	if (*str == '-') {
		str++;
		sign = -1;
	}
	else if (*str == '+')
		str++;

	while (*str != '\0' && isdigit(*str)) {
		num = num * 10 + (*str) - '0';
		str++;
	}

	return num * sign;
}
