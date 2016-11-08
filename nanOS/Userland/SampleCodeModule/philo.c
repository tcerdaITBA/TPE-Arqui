#include "philo.h"
#include "time.h"
#include "stdio.h"
#include "syscalls.h"
#include "strings.h"

int critical_m;
int state[MAX_PHILOSOPHERS];
int mut[MAX_PHILOSOPHERS];
int philosopherCount = 0;

static void take_forks(uint64_t i);
static void put_forks(uint64_t i);
static void test(uint64_t i);
static void philosopher(uint64_t i);
static void setState(int philo, int st);

int start_philosophers_problem(int philoNumber) {
  int i;
  critical_m = mutex_open("MainPhilosophersMutex");
  philosopherCount = philoNumber;

  // Set inicial de filosofos
  for (i = 0; i < philosopherCount; i++) {
    char name[] = "PhilosopherMutex000";
    name[str_len(name)-1] = i + '0';
    mut[i] = mutex_open(name);
    mutex_lock(mut[i]);
    state[i] = THINKING;
  }

  for (i = 0; i < philosopherCount; i++) {
    exec(philosopher, i);
  }

  return 0;
}

static void philosopher(uint64_t i) {
  while(1) {
    take_forks(i);
    put_forks(i);
  }
}

static void take_forks(uint64_t i) {
  mutex_lock(critical_m);
  setState(i, HUNGRY);
  test(i);
  mutex_unlock(critical_m);
  mutex_lock(mut[i]);
}

static void put_forks(uint64_t i) {
  mutex_lock(critical_m);
  if (state[i] == HUNGRY)
    printf("EXPLOTAR\n");

  setState(i, THINKING);
  test(LEFT(i, philosopherCount));
  test(RIGHT(i, philosopherCount));
  mutex_unlock(critical_m);
}

static void setState(int philo, int st) {
  state[philo] = st;
  render(state, philosopherCount);
  sleep(3000); // TODO ver donde va
}

static void test(uint64_t i) {
  if (state[i] == HUNGRY && state[LEFT(i, philosopherCount)] != EATING \
    && state[RIGHT(i, philosopherCount)] != EATING) {
    setState(i, EATING);
    mutex_unlock(mut[i]);
  }
}
