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

  printf("EMPTY_FD: %d\n", empty_fd);
  printf("FULL_FD: %d\n", full_fd);

  while(1) {
    printf("Producer about to read empty\n");
    read(empty_fd,  &message, 1);

    sleep(5000 - PRODUCER_SPEED * 1000);
    printf("PRODUCING - read %c\n", message);

    printf("Producer about to write full\n");
    write(full_fd, &full, 1);
  }
}

static void consumer() {
  printf("Consumer Starting...\n");
  char message;
  char empty = EMPTY_SLOT;
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int full_fd = fifo_open(CONSUMER_FIFO);

  printf("EMPTY_FD: %d\n", empty_fd);
  printf("FULL_FD: %d\n", full_fd);

  while(1) {
    printf("Consumer about to read full\n");
    read(full_fd,  &message, 1);

    sleep(35000 - CONSUMER_SPEED * 1000);
    printf("CONSUMING - read %c\n", message);

    printf("Consumer about to write empty\n");
    write(empty_fd, &empty, 1);
  }
}

void start_producer_consumer_problem() {
  char empty = EMPTY_SLOT;
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int i;
  for (i = 0; i < N; i++)
    write(empty_fd, &empty, 1);

  printf("-- Starting -- \n\n");
  exec(producer, 0);
  sleep(10000);
  exec(consumer, 0);
}
