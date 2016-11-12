#include "stdio.h"
#include "philo.h"
#include "time.h"

char * stateStrings[3] = {"THINKING", "HUNGRY", "EATING"};

void render(int * philoState, int philosopherCount) {
	for(int i = 0; i < philosopherCount; i++) {
		printf("Philosopher %d: %s\n", i, stateStrings[philoState[i]]);

		if (philoState[i] == EATING && \
			philoState[i] == philoState[(i+1) % philosopherCount]) {
			printf("\n\n- ESTA TODO ROTO ALERTA -\n\n");
			// while(1); // frenar todo
		}
	}

	putchar('\n');
	sleep(500);
}
