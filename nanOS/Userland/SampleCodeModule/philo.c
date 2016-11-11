#include "philo.h"
#include "time.h"
#include "stdio.h"
#include "syscalls.h"
#include "strings.h"
#include "random.h"

int critical_m;
int state[MAX_PHILOSOPHERS];
int mut[MAX_PHILOSOPHERS];
int philosopherCount = 0;

static void take_forks(uint64_t i);
static void put_forks(uint64_t i);
static void test(uint64_t i);
static void philosopher(uint64_t i);
static void setState(int philo, int st);
static void listen_commands();

int start_philosophers_problem(int philoNumber) {
  int i;
  philosopherCount = philoNumber > MAX_PHILOSOPHERS ? MAX_PHILOSOPHERS : philoNumber;

  critical_m = mutex_open("MainPhilosophersMutex");

  // Set inicial de filosofos
  for (i = 0; i < philosopherCount; i++) {
    char name[] = "PhilosopherMutex000";
    name[strlen(name)-1] = i + '0';
    mut[i] = mutex_open(name);
    mutex_lock(mut[i]);
    state[i] = THINKING;
  }

  srand(seconds() * minutes() * hour());

  for (i = 0; i < philosopherCount; i++) {
    exec(philosopher, i);
  }

  // listen_commands(); falta ponerlo en foreground
  return 0;
}

static void listen_commands() {
  char c;
  while((c = getchar())) {
    switch (c) {
      case 'e':
      return;
      break;
      case 'a':
      philosopherCount += 1;
      exec(philosopher, philosopherCount - 1);
      break;
      case 'r':
      philosopherCount -= 1;
      // kill al ultimo filosofo
      break;
    }
  }
}

static void philosopher(uint64_t i) {
  while(1) {
    sleep(rand_int_range(5, 20) * 1000); // No se si anda bien el sleep
    take_forks(i);
    sleep(rand_int_range(5, 20) * 1000); //
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
}

static void test(uint64_t i) {
  if (state[i] == HUNGRY && state[LEFT(i, philosopherCount)] != EATING \
    && state[RIGHT(i, philosopherCount)] != EATING) {
    setState(i, EATING);
    mutex_unlock(mut[i]);
  }
}
