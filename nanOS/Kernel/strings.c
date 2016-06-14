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

int strcpynto(char * to, char * from, char limit, int n) {
	int i;
	for (i = 0; i < n && from[i] != limit; i++) {
		to[i] = from[i];
	}
	return i;
}
