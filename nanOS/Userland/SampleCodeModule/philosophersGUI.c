#include "stdio.h"
#include "philo.h"
#include "time.h"
#include "graphics.h"

char * stateStrings[3] = {"THINKING", "HUNGRY", "EATING"};

void renderText(int * philoState, int philosopherCount) {
	if (philosopherCount == 0)
		printf("There are no philosophers left\n");
	else {
		for(int i = 0; i < philosopherCount; i++) {
			printf("Philosopher %d: %s\n", i, stateStrings[philoState[i]]);
		}
		putchar('\n');
	}

	sleep(500);
}

void renderGraphics(int * philoState, int philosopherCount) {
	int i;
	int r, g, b;
|	double dishAngle, dishXpos, dishYpos;
	double dishSpace = (2 * PI * TABLE_RADIUS) / philosopherCount;
	double dishRadius = dishSpace * 0.8;

	for (i = 0; i < philosopherCount; i++) {
		dishAngle = (2 * PI * i) / philosopherCount;
		dishXpos = TABLE_RADIUS * cos(dishAngle) + TABLE_X_POS;
		dishYpos = TABLE_RADIUS * sin(dishAngle) + TABLE_Y_POS;

		get_state_color(philoState[i], &r, &g, &b);

		draw_filledCircle(dishXpos, dishYpos, dishRadius, );
	}
}

void get_state_color(int philoState, int * r, int * g, int * b) {
	switch(philoState) {
		case EATING:
		*r = 255; *g = 0; *b = 0;
		break;
		case HUNGRY:
		*r = 255; *g = 187; *b = 0;
		break;
		case THINKING:
		*r = 112; *g = 142; *b = 224;
		break;
	}
}