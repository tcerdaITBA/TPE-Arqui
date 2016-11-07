#include "philo.h"
#include "time.h"
#include "stdio.h"
#include "syscalls.h"

int critical_m;
int state[N];
int mut[N];

static void take_forks(uint64_t i);
static void put_forks(uint64_t i);
static void test(uint64_t i);
static void philosopher(uint64_t i);
static int sleep_time();

static char * stateNames[3] = {"THINKING", "HUNGRY", "EATING"};

int start_philosophers_problem() {
  critical_m = mutex_open("MainMutex");
  int i;
  printf("critical: %d\n", critical_m);

  for (i = 0; i < N; i++) {
    char aux[] = "Mute";
    aux[3] = i + '0';
    mut[i] = mutex_open(aux);
    mutex_lock(mut[i]);
    state[i] = THINKING;
    printf("mut{%d}: %d\n", i, mut[i]);
  }

  for (i = 0; i < N; i++) {
    sleep(2000);
    exec(philosopher, i);
  }

  return 0;
}

static void philosopher(uint64_t i) {
  while(1) {
    //think(i);
    take_forks(i);
    sleep(sleep_time());
  //  eat(i);
    put_forks(i);
    sleep(sleep_time());
  }
}

static void take_forks(uint64_t i) {
  mutex_lock(critical_m);
  printf("PHILO [%d] %s -> %s\n", i, stateNames[state[i]], stateNames[HUNGRY]);
  state[i] = HUNGRY;
  test(i);
  mutex_unlock(critical_m);
  mutex_lock(mut[i]);
}

static void put_forks(uint64_t i) {
  mutex_lock(critical_m);
  printf("PHILO [%d] %s -> %s\n", i, stateNames[state[i]], stateNames[THINKING]);
  if (state[i] == HUNGRY) 
    printf("EXPLOTAR\n");
  state[i] = THINKING;
  test(LEFT(i));
  test(RIGHT(i));
  mutex_unlock(critical_m);
}

static void test(uint64_t i) {
  if (state[i] == HUNGRY && state[LEFT(i)] != EATING && state[RIGHT(i)] != EATING) {
    printf("PHILO [%d] %s -> %s\n", i, stateNames[state[i]], stateNames[EATING]);
    state[i] = EATING;
    mutex_unlock(mut[i]);
  }
}

static int sleep_time() {
  return ((seconds() % N) * 1000) + 3000;
}