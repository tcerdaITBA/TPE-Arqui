#ifndef _PHILO_H_
#define _PHILO_H_

#define LEFT(x, N) (((x)+(N)-1) % (N != 0 ? N : 1))
#define RIGHT(x, N) (((x)+1) % (N != 0 ? N : 1))
#define THINKING 0
#define HUNGRY 1
#define EATING 2

#define UNPAUSED 1
#define PAUSED 0

#define MAX_PHILOSOPHERS 32

#define MAIN_MUTEX_NAME "MAIN_PHILOSOPHERS_MUTEX"

#define TABLE_X_POS 300
#define TABLE_Y_POS 300
#define TABLE_RADIUS 100

int start_philosophers_problem(int philoNumber);

void renderText(int * philoState, int philosopherCount);

void renderGraphics(int * philoState, int philosopherCount);

#endif
