#include "stdio.h"
#include "philo.h"
#include "time.h"

char * stateStrings[3] = {"THINKING", "HUNGRY", "EATING"};

void renderProcess(int * philoState, int philosopherCount) {
	for(int i = 0; i < philosopherCount; i++) {
		printf("Philosopher %d: %s\n", i, stateStrings[philoState[i]]);
	}

	putchar('\n');
	sleep(500);
}
