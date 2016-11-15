#include "stdio.h"
#include "philo.h"
#include "time.h"
#include "graphics.h"
#include "math.h"

char * stateStrings[] = {"THINKING", "HUNGRY", "EATING", "DYING"};

int prevPhilosopherCount;

static void get_state_color(int philoState, int * r, int * g, int * b);

static void cleanTable(int x, int y, double offset);

void renderText(philosopher_data * philos, int philosopherCount) {
	if (philosopherCount == 0)
		printf("There are no philosophers left\n");
	else {
		for(int i = 0; i < philosopherCount; i++) {
			printf("Philosopher %d: %s\n", i, stateStrings[philos[i].state]);
		}
		putchar('\n');
	}

	sleep(500);
}

void renderGraphics(philosopher_data * philos, int philosopherCount) {
	if (philosopherCount == 0) {
		printf("There are no philosophers left\n");
		return;
	}

	int i;
	int r, g, b;
	double dishAngle, dishXpos, dishYpos;
	double dishSpace = (2 * PI * TABLE_RADIUS) / philosopherCount;
	double dishRadius = dishSpace * 0.35;

	if (prevPhilosopherCount != philosopherCount)
		cleanTable(TABLE_X_POS, TABLE_Y_POS, dishRadius);

	prevPhilosopherCount = philosopherCount;

	for (i = 0; i < philosopherCount; i++) {
		dishAngle = (2 * PI * i) / philosopherCount;
		dishXpos = TABLE_RADIUS * cos(dishAngle) + TABLE_X_POS;
		dishYpos = TABLE_RADIUS * sin(dishAngle) + TABLE_Y_POS;

		get_state_color(philos[i].state, &r, &g, &b);

		draw_filledCircle2(dishXpos, dishYpos, dishRadius, r, g, b);
	}
}

static void get_state_color(int philoState, int * r, int * g, int * b) {
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
		case DYING:
		*r = 25; *g = 25; *b = 25;
		break;
	}
}

static void cleanTable(int x, int y, double offset) {
	draw_filledCircle2(x, y, TABLE_RADIUS + offset*2, 0, 0 ,0);
}
