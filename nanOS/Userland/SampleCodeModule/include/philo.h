#ifndef _PHILO_H_
#define _PHILO_H_

#define N 5
#define LEFT(x) ((x)-1)%N
#define RIGHT(x) ((x)+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define SLEEP_STEP 5000

int start_philosophers_problem();

#endif
