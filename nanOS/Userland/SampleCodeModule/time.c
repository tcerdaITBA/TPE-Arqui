#include "syscalls.h"
#include "stdio.h"
#include "time.h"

int GMT = DEFAULT_GMT;

/*
** Configura el GMT del reloj.
** Retorna 1 si es un GMT valido, 0 sino.
*/
int setGMT(int gmt) {
	if (gmt >= MIN_GMT && gmt <= MAX_GMT) {
		GMT = gmt;
		return 1;
	}
	return 0;
}

int getGMT(int gmt) {
	return GMT;
}

int seconds() {
    return time(0);
}

int minutes() {
    return time(1);
}

int hour() {
    return (time(2) + GMT) % 24;
}
