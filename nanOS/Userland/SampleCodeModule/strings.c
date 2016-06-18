#include "strings.h"
#include "limits.h"
#include "ctype.h"
#include "stdlib.h"

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