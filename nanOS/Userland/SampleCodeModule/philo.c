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
int philosophers_PID[MAX_PHILOSOPHERS];

static void take_forks(int i);
static void put_forks(int i);
static void test(int i);
static void philosopher(int argc, char * argv[]);
static void setState(int philo, int st);

static int add_philosopher();
static void remove_philosopher();
static void remove_all_philosophers();

void listen_commands();

int start_philosophers_problem(int philoNumber) {
  int i;
  philoNumber = philoNumber > MAX_PHILOSOPHERS ? MAX_PHILOSOPHERS : philoNumber;

  critical_m = mutex_open("MainPhilosophersMutex");
  srand(seconds() * minutes() * hour());

  for (i = 0; i < philoNumber; i++) {
    philosophers_PID[i] = add_philosopher();
  }

  int pid = execpn(listen_commands);
  set_foreground(pid);
  // TODO: Devolverle el foreground a la shell
  return 0;
}

void listen_commands() {
  char c;
  while((c = getchar())) {
    switch (c) {
      case 'e':
      remove_all_philosophers();
      return;
      break;
      case 'a':
      add_philosopher();
      break;
      case 'r':
      remove_philosopher();
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

static int add_philosopher() {
  char name[] = "PhilosopherAddMutex000";
  char args[3];
  int new_pid = -1, philo_index;

  mutex_lock(critical_m);
  if (philosopherCount < MAX_PHILOSOPHERS) {
    philo_index = philosopherCount;
    philosopherCount += 1;

    name[strlen(name)-1] = '0' + philo_index;
    mut[philo_index] = mutex_open(name);

    printf("ME CLAVE\n");
    mutex_lock(mut[philo_index]);
    printf("ME DESCLAVE\n");

    state[philo_index] = THINKING;
    itoa(philo_index, args, 10);

    new_pid = execp(philosopher, args);
  }
  mutex_unlock(critical_m);
  return new_pid;
}

static void remove_philosopher() {
  mutex_lock(critical_m);
  if (philosopherCount > 0) {
    philosopherCount -= 1;
    printf("Killing %d - PID: %d\n", philosopherCount, philosophers_PID[philosopherCount]);
    printf("About to close mutex\n");
    mutex_close(mut[philosopherCount]);
    printf("About to kill philosopher\n");
    kill(philosophers_PID[philosopherCount]);
    printf("Killed it\n");
  }
  mutex_unlock(critical_m);
}

static void remove_all_philosophers() {
  int i, philoCountAux = philosopherCount;
  for (i = 0; i < philoCountAux; i++)
    remove_philosopher();
}

static void philosopher(int argc, char * argv[]) {
  int i = atoi(argv[0]);
  while(1) {
    printf("philosopher %d is alive\n", i);
    sleep(rand_int_range(1, 5) * 1000);
    take_forks(i);
    sleep(rand_int_range(1, 5) * 1000);
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
