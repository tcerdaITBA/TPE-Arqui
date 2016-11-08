#include "stdio.h"
#include "philo.h"

char * stateStrings[3] = {"THINKING", "HUNGRY", "EATING"};

void render(int * philoState, int philosopherCount) {
	for(int i = 0; i < philosopherCount; i++) {
		printf("Philosopher %d: %s\n", i, stateStrings[philoState[i]]);
		// printf("Fork - ");
		//
		// if (forkState[i] == -1)
		// 	printf("Free\n");
		// else
		// 	printf("Owner %d\n", forkState[forkState[i]]);
	}

	putchar('\n');
}

// void setPhiloState(int philo, int state) {
// 	philoState[philo] = state;
// }
//
// void setForkState(int fork, int owner) {
// 	forkState[fork] = owner;
// }
