#ifndef _PHILO_H_
#define _PHILO_H_

#define LEFT(x, N) (((x)+(N)-1) % (N))
#define RIGHT(x, N) (((x)+1) % (N))
#define THINKING 0
#define HUNGRY 1
#define EATING 2

#define MAX_PHILOSOPHERS 32

#define MAIN_MUTEX_NAME "MAIN_PHILOSOPHERS_MUTEX"

int start_philosophers_problem(int philoNumber);

void render(int * philoState, int philosopherCount);

#endif
