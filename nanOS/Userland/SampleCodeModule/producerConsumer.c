#include "producerConsumer.h"
#include "stdio.h"
#include "time.h"

#define CONSUMER_SPEED 1
#define PRODUCER_SPEED 4

static void producer();
static void consumer();

static void producer() {
  printf("Producer Starting...\n");

  char message;
  char full = FULL_SLOT;
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int full_fd = fifo_open(CONSUMER_FIFO);

  // printf("EMPTY_FD: %d\n", empty_fd);
  // printf("FULL_FD: %d\n", full_fd);

  while(1) {
    read(empty_fd,  &message, 1);
    printf("PRODUCING\n");
    sleep(400);
    write(full_fd, &full, 1);
  }
}

static void consumer() {
  printf("Consumer Starting...\n");
  char message;
  char empty = EMPTY_SLOT;
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int full_fd = fifo_open(CONSUMER_FIFO);

  // printf("EMPTY_FD: %d\n", empty_fd);
  // printf("FULL_FD: %d\n", full_fd);

  while(1) {
    read(full_fd,  &message, 1);
    printf("CONSUMING\n");
    sleep(700);
    write(empty_fd, &empty, 1);
  }
}

// TODO: que pida por parametro el tamaño del buffer
void start_producer_consumer_problem() {
  char empty = EMPTY_SLOT;
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int i;
  for (i = 0; i < 100; i++)
    write(empty_fd, &empty, 1);

  execpn(producer);
  execpn(consumer);

  // Proceso que escucha si se acelera el producer o el consumer
}
