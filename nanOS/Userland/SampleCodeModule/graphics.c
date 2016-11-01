#include "paint.h"
#include "stdlib.h"
#include "syscalls.h"

#define OK 0
#define ERROR 1

int draw_circle(int x, int y, int radius) {

	int currX,currY;
/*
	for(currX= x-radius, currX <= x + radius; currX++){
		
	}
*/
	return OK;
}

//TODO: cambiar el color
int draw_horizontalLine(int x, int y, int lenght) {
	int i;
	for(i = 0; i <= lenght ; i++) {
		paint_pixel(255,255,255,x+i, y);
	}
	return OK;
}