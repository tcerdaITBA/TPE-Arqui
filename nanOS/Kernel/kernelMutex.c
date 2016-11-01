typedef struct {
  process * p;
  mutex_node * next;
} mutex_node;

typedef struct {
  mutex_node * first;
  mutex_node * last;
} mutex_queue;

typedef struct {
  char * name;
  int locked;
  mutex_queue process_queue;
} mutex;

#define LOCKED = 1;
#define UNLOCKED = 0;

#define BLOCKED = 34;
#define RUNNING = 35;

#define MAX_MUTEXES = 64;

#define NOT_OPEN = -1;

static mutex open_mutexes[MAX_MUTEXES];

int is_open(int key) {
  return key < MAX_MUTEXES && open_mutexes[key] != NULL;
}

int mutex_open(char * name) {
  int k, free_slot = -1;
  for (k = 0; k < MAX_MUTEXES; k++) {
    if (!is_open(k)) {
      free_slot = k;
    }
    else if (strcmp(name, open_mutexes[k].name) == 0) {
      return k;
    }
  }
  mutex_queue pq;
  open_mutexes[free_slot] = {name, UNLOCKED, pq};
  return free_slot;
}

// Si hay procesos lockeados?
int mutex_close(int key) {
  if (is_open(key)) {
    open_mutexes[key] = NULL;
    return 1;
  }
  return NOT_OPEN;
}

int mutex_lock(int key) {
  if (is_open(key)) {
    //if !testnset
    mutex m = open_mutexes[key];
    process p = get_current_process(); // context switch?
    block_process(p);
    queue_process(m, p);
    return 1;
  }
  return NOT_OPEN;
}

int mutex_unlock(int key) {
  if (is_open(key)) {
    mutex m = open_mutexes[key];
    m.locked = UNLOCKED;
    process p = dequeue_process(m);
    unblock_process(p);
    return 1;
  }
  return NOT_OPEN;
}

void block_process(process p) {
  p.status = BLOCKED;
}

void unblock_process(process p) {
  p.status = RUNNING;
}

void queue_process(mutex m, process p) {
  mutex_node = malloc();
  mutex_node->p = p;
  mutex_node->next = NULL;
  m.process_queue->last->next = mutex_node;
  m.process_queue->last = mutex_node;
}

process dequeue_process(mutex m) {
  process p = m.process_queue->first->p;
  m.process_queue->first = m.process_queue->first->next;
  return p;
}
