#include "syscalls.h"

#define N 5
#define LEFT(x) ((x)-1)%N
#define RIGHT(x) ((x)+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int critical_m;
int state[N];
int mut[N];

void philosopher(int i);
void take_forks(int i);
void put_forks(int i);
void test(int i);
void eat();
void think();

int start_philosophers_problem(int argc, char const *argv[]) {
  critical_m = mutex_open("MainMutex");
  int i;
  for (i = 0; i < N; i++) {
    char aux[] = "Mute";
    aux[3] = i + '0';
    mut[i] = mutex_open(aux);
  }

  return 0;
}

void philosopher(int i) {
  while(1) {
    think();
    take_forks(i);
    eat();
    put_forks(i);
  }
}

void think() {
  //thinking
}
void eat() {
  //eating
}

void take_forks(int i) {
  mutex_lock(critical_m);
  state[i] = HUNGRY;
  test(i);
  mutex_unlock(critical_m);
  mutex_lock(mut[i]);
}

void put_forks(int i) {
  mutex_lock(critical_m);
  state[i] = THINKING;
  test(LEFT(i));
  test(RIGHT(i));
  mutex_unlock(critical_m);
}

void test(int i) {
  if (state[i] == HUNGRY && state[LEFT(i)] != EATING && state[RIGHT(i)] != EATING) {
    state[i] = EATING;
    mutex_unlock(mut[i]);
  }
}
