#include "memoryAllocator.h"
#include "processManager.h"
#include "kernelMutex.h"
#include "strings.h"
#include "process.h"
#include "queueADT.h"

typedef struct {
  char name[MUTEX_NAME_LEN];
  int64_t locked;
  int64_t queue_lock;
  int64_t state;
  queueADT process_queue;
} mutex;


static mutex open_mutexes[MAX_MUTEXES];
static int64_t array_mutex = UNLOCKED;

extern int _unlocked(int64_t * locknum);

static process * dequeue_process(mutex *m);
static void queue_process(mutex *m, process * p);

static mutex create_new_mutex(char * name);
static int is_open(int key);


static void lock_queue(mutex *m);
static void unlock_queue(mutex *m);

static void lock_array();
static void unlock_array();


static int is_open(int key) {
  return key < MAX_MUTEXES && open_mutexes[key].state == OPEN;
}

static void lock_queue(mutex *m) {
  while (!_unlocked(&m->queue_lock))
    yield_process();
}

static void unlock_queue(mutex *m) {
  m->queue_lock = UNLOCKED;
}

static void lock_array() {
  while (!_unlocked(&array_mutex))
    yield_process();
}

static void unlock_array() {
  array_mutex = UNLOCKED;
}

int mutex_open(char * name) {
  int k;

  lock_array();

  for (k = 0; k < MAX_MUTEXES && is_open(k); k++) {
    if (strcmp(name, open_mutexes[k].name) == 0) {
      unlock_array();
      return k;
    }
  }

  if (k == MAX_MUTEXES)
    return MAX_MUTEX_OPEN_ERROR;

  open_mutexes[k] = create_new_mutex(name);

  unlock_array();

  return k;
}

static mutex create_new_mutex(char * name) {
  mutex m;
  strcpy(m.name, name);
  m.process_queue = create_queue();
  m.state = OPEN;
  m.locked = UNLOCKED;
  m.queue_lock = UNLOCKED;
  return m;
}

int mutex_close(int key) {
  if (is_open(key)) {
    lock_array();

    mutex * m = &open_mutexes[key];

    while (!is_empty(m->process_queue)) // Hay procesos lockeados
      unblock_process(dequeue_process(m));

    m->state = CLOSED;

    unlock_array();
    return 1;
  }
  return NOT_OPEN_ERROR;
}

int mutex_lock(int key) {
  if (is_open(key)) {

    mutex *m = &open_mutexes[key];

    process * p = get_current_process();

    assign_quantum(); /* Si hay cambios de contexto debajo se rompe todo */

    if (!_unlocked(&m->locked)) {

      lock_queue(m);

      queue_process(m, p);

      block_process(p);

      unlock_queue(m);


      yield_process();
    }
    else
      unassign_quantum();

    return 1;
  }
  return NOT_OPEN_ERROR;
}

int mutex_unlock(int key) {
  if (is_open(key)) {
    mutex *m = &open_mutexes[key];

    assign_quantum();

    lock_queue(m);

    process * p = dequeue_process(m);

    m->locked = UNLOCKED;

    if (p != NULL) {
      unblock_process(p);
      m->locked = LOCKED;
    }

    unlock_queue(m);

    unassign_quantum();

    return 1;
  }
  return NOT_OPEN_ERROR;
}

static void queue_process(mutex *m, process * p) {
  enqueue(m->process_queue, p);
}

static process * dequeue_process(mutex *m) {
  return dequeue(m->process_queue);
}
