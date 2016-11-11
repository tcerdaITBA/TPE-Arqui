#include "philo.h"
#include "time.h"
#include "stdio.h"
#include "syscalls.h"
#include "strings.h"
#include "random.h"
#include "executer.h"
#include "stdlib.h"

int critical_m;
int state[MAX_PHILOSOPHERS];
int mut[MAX_PHILOSOPHERS];
int philosopherCount = 0;

static void take_forks(int i);
static void put_forks(int i);
static void test(int i);
static void philosopher(int argc, char * argv[]);
static void setState(int philo, int st);

static void add_philosopher();

void listen_commands();

int start_philosophers_problem(int philoNumber) {
  int i;
  philoNumber = philoNumber > MAX_PHILOSOPHERS ? MAX_PHILOSOPHERS : philoNumber;

  critical_m = mutex_open("MainPhilosophersMutex");
  srand(seconds() * minutes() * hour());

  for (i = 0; i < philoNumber; i++) {
    add_philosopher();
  }

  int pid = execpn(listen_commands);
  set_foreground(pid);
  return 0;
}

void listen_commands() {
  char c;
  while((c = getchar())) {
    switch (c) {
      case 'e':
      // kill all philos
      end();
      break;
      case 'a':
      add_philosopher();
      break;
      case 'r':
      //philosopherCount -= 1;
      // kill al ultimo filosofo
      break;
      case 'p':
      mutex_lock(critical_m);
      break;
      case 'u':
      mutex_unlock(critical_m);
      break;
    }
  }
}

static void add_philosopher() {
  mutex_lock(critical_m);
  char name[] = "PhilosopherAddMutex000";
  char args[3];
  int philo_index = philosopherCount;
  philosopherCount += 1;

  name[strlen(name)-1] = '0' + philo_index;
  mut[philo_index] = mutex_open(name);
  mutex_lock(mut[philo_index]);

  state[philo_index] = THINKING;
  itoa(philo_index, args, 10);

  execp(philosopher, args);

  mutex_unlock(critical_m);
}

static void philosopher(int argc, char * argv[]) {
  int i = atoi(argv[0]);
  while(1) {
    sleep(rand_int_range(5, 10) * 1000); // No se si anda bien el sleep
    take_forks(i);
    sleep(rand_int_range(5, 20) * 1000); //
    put_forks(i);
  }
}

static void take_forks(int i) {
  mutex_lock(critical_m);
  setState(i, HUNGRY);
  test(i);
  mutex_unlock(critical_m);
  mutex_lock(mut[i]);
}

static void put_forks(int i) {
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

static void test(int i) {
  if (state[i] == HUNGRY && state[LEFT(i, philosopherCount)] != EATING \
    && state[RIGHT(i, philosopherCount)] != EATING) {
    setState(i, EATING);
    mutex_unlock(mut[i]);
  }
}
