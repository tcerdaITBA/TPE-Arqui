#include "memoryAllocator.h"
#include "processManager.h"
#include "kernelMutex.h"
#include "strings.h"
#include "process.h"
#include "videoDriver.h"

#define NULL ((void *) 0)

typedef struct mutex_node {
  process * p;
  struct mutex_node * next;
} mutex_node_t;

typedef struct {
  mutex_node_t * first;
  mutex_node_t * last;
} mutex_queue;

typedef struct {
  char name[MUTEX_NAME_LEN];
  int64_t locked;
  int64_t state;
  mutex_queue process_queue;
} mutex;


static mutex open_mutexes[MAX_MUTEXES];

extern int _unlocked(int64_t * locknum);

static process * dequeue_process(mutex *m);
static void queue_process(mutex *m, process * p);

static mutex create_new_mutex(char * name);
static int is_open(int key);

static int is_open(int key) {
  return key < MAX_MUTEXES && open_mutexes[key].state == OPEN;
}


int mutex_open(char * name) {
  int k;

  for (k = 0; k < MAX_MUTEXES && is_open(k); k++) {
    if (strcmp(name, open_mutexes[k].name) == 0) {
      return k;
    }
  }

  if (k == MAX_MUTEXES)
    return MAX_MUTEX_OPEN_ERROR;
  
  open_mutexes[k] = create_new_mutex(name);

  return k;
}

static mutex create_new_mutex(char * name) {
  mutex m;
  strcpy(m.name, name);
  m.process_queue.first = NULL;
  m.process_queue.last = NULL;
  m.state = OPEN;
  m.locked = UNLOCKED;
  return m;
}

//VER
int mutex_close(int key) {
  if (is_open(key)) {
    open_mutexes[key].state = CLOSED;
    return 1;
  }
  return NOT_OPEN_ERROR;
}

int mutex_lock(int key) {
  if (is_open(key)) {
    mutex *m = &open_mutexes[key];

    if (!_unlocked(&m->locked)) {
      process * p = get_current_process();
      block_process(p);
      queue_process(m, p);
      yield_process();
    }

    return 1;
  }
  return NOT_OPEN_ERROR;
}

int mutex_unlock(int key) {
  if (is_open(key)) {
    mutex *m = &open_mutexes[key];
    m->locked = UNLOCKED;
    process * p = dequeue_process(m);
    if (p != NULL)
      unblock_process(p);
    return 1;
  }
  return NOT_OPEN_ERROR;
}

static void queue_process(mutex *m, process * p) {
  mutex_node_t * node = (mutex_node_t *) get_page(sizeof(*node));
  node->p = p;

  if (m->process_queue.first == NULL) {
    m->process_queue.first = node;
    m->process_queue.last = node;
  }
  else {
    m->process_queue.last->next = node;
    m->process_queue.last = node;
  }

}

static process * dequeue_process(mutex *m) {
  if (m->process_queue.first == NULL)
    return NULL;

  mutex_node_t * node = m->process_queue.first;

  process * p = m->process_queue.first->p;
  m->process_queue.first = m->process_queue.first->next;

  store_page((uint64_t) node);

  return p;
}