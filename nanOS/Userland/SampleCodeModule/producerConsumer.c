#include "producerConsumer.h"
#include "stdio.h"
#include "time.h"
#include "executer.h"

static void producer();
static void consumer();
static void control_speed();
static void print_speeds();

int producerSleep = INITIAL_PROD_SLEEP;
int consumerSleep = INITIAL_CONS_SLEEP;

static void producer() {
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int full_fd = fifo_open(CONSUMER_FIFO);

  char message;
  char full = FULL_SLOT;

  while(1) {
    read(empty_fd,  &message, 1);
    printf("Produce\n");
    sleep(producerSleep * SLEEP_MULTIPLIER);
    write(full_fd, &full, 1);
  }
}

static void consumer() {
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int full_fd = fifo_open(CONSUMER_FIFO);

  char message;
  char empty = EMPTY_SLOT;

  while(1) {
    read(full_fd,  &message, 1);
    printf("Consume\n");
    sleep(consumerSleep * SLEEP_MULTIPLIER);
    write(empty_fd, &empty, 1);
  }
}

void start_producer_consumer_problem(int buf_size) {
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int full_fd = fifo_open(CONSUMER_FIFO);
  int prod_pid, cons_pid;
  int i;
  char empty = EMPTY_SLOT;

  for (i = 0; i < buf_size; i++)
    write(empty_fd, &empty, 1);

  prod_pid = execpn(producer);
  cons_pid = execpn(consumer);

  control_speed();
  kill(prod_pid);
  kill(cons_pid);
  fifo_close(empty_fd);
  fifo_close(full_fd);
}

static void control_speed() {
  char c;
  while((c = getchar())) {
    switch (c) {
      case 'a':
        producerSleep++;
        print_speeds();
      break;
      case 's':
        producerSleep = producerSleep > 0 ? producerSleep-1 : 0;
        print_speeds();
      break;
      case 'z':
        consumerSleep++;
        print_speeds();
      break;
      case 'x':
        consumerSleep = consumerSleep > 0 ? consumerSleep-1 : 0;
        print_speeds();
      break;
      case 'q':
      return;
      break;
    }
  }
}

static void print_speeds() {
  printf("Producer sleep: %d\n", producerSleep);
  printf("Consumer sleep: %d\n", consumerSleep);
}
