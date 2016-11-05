#include "fifo.h"
#include "process.h"
#include "processManager.h"
#include "kernelMutex.h"
#include "queueADT.h"
#include "strings.h"

typedef struct {
  char name[FIFO_NAME_LEN];
  char buffer[BUF_SIZE];
  int state;
  int buf_fill;
  int fifo_mutex_key;
  queueADT read_queue;
} fifo;

typedef struct {
  int bytes;
  void * buffer;
  fifo f;
  process * reader_p;
} read_request;

static fifo open_fifos[MAX_FIFOS];

static int try_read(read_request r);
static fifo create_new_fifo(char * name);
static void send_to_readers(queueADT read_queue);
static read_request create_read_request(int fifo_key, void * buf, int bytes);
static int is_open(int key);
static void release_readers(queueADT q);

int fifo_open(char * name) {
  int k;

  for (k = 0; k < MAX_FIFOS && is_open(k); k++) {
    if (strcmp(name, open_fifos[k].name) == 0) {
      return k;
    }
  }

  if (k == MAX_FIFOS)
    return MAX_FIFOS_OPEN_ERROR;

  open_fifos[k] = create_new_fifo(name);

  return k;
}

static fifo create_new_fifo(char * name) {
  fifo f;
  strcpy(f.name, name);
  f.buf_fill = 0;
  f.state = OPEN;
  f.fifo_mutex_key = mutex_open(name); // TODO: Ver que nombre pasarle.
  f.read_queue = create_queue();
  return f;
}

static read_request create_read_request(int fifo_key, void * buf, int bytes) {
  read_request r;
  r.bytes = bytes;
  r.buffer = buf;
  r.f = open_fifos[fifo_key];
  r.reader_p = get_current_process();
  return r;
}

static int is_open(int key) {
  return key < MAX_FIFOS && open_fifos[key].state == OPEN;
}

int fifo_write(int key, void * buf, int bytes) {
  int write_bytes;
  fifo f;
  if (is_open(key)) {
    f = open_fifos[key];
    mutex_lock(f.fifo_mutex_key);

    write_bytes = (f.buf_fill + bytes) > BUF_SIZE ? BUF_SIZE - f.buf_fill : bytes;

    strcpynto(f.buffer + f.buf_fill, buf, '\0',write_bytes);
    f.buf_fill += write_bytes;

    send_to_readers(f.read_queue);

    mutex_unlock(f.fifo_mutex_key);
    return write_bytes;
  }
  return FIFO_NOT_OPEN_ERROR;
}

static void send_to_readers(queueADT read_queue) {
  if (!is_empty(read_queue)) {
    while(try_read(*((read_request *)peek(read_queue)))) {
      // manda a los lectores hasta que uno no pueda leer mas
      dequeue(read_queue); // leyo, entonces lo saca de los que estan esperando leer
    }
  }
}

int fifo_read(int key, void * buf, int bytes) {
  int could_read;
  if (is_open(key)) {
    fifo f = open_fifos[key];
    mutex_lock(f.fifo_mutex_key);

    read_request r = create_read_request(key, buf, bytes);
    if (is_empty(f.read_queue)) // primer reader
      could_read = try_read(r);
    else // mas de un reader esperando
      enqueue(f.read_queue, (void *)&r);

    mutex_unlock(f.fifo_mutex_key);

    if (!could_read)
      yield_process(); // esta bloqueado

    if (!is_open(key)) // se desbloqueo porque cerraron el fifo;
      return FIFO_NOT_OPEN_ERROR;

    return bytes;
  }
  return FIFO_NOT_OPEN_ERROR;
}

int fifo_close(int key) {
  if (is_open(key)) {
    fifo f = open_fifos[key];
    release_readers(f.read_queue);
    open_fifos[key].state = CLOSED;
    return 1;
  }
  return FIFO_NOT_OPEN_ERROR;
}

static void release_readers(queueADT q) {
  read_request * req;
  while (!is_empty(q)) {
    req = (read_request *) dequeue(q);
    unblock_process(req->reader_p);
  }
}

static int try_read(read_request r) {
  if (r.f.buf_fill >= r.bytes) {  // el buffer tenga los bytes que quiero leer
    strcpynto(r.buffer, r.f.buffer, '\0', r.bytes);
    unblock_process(r.reader_p);
    return 1; // lee
  }
  else {
    block_process(r.reader_p);
    return 0; // no lee
  }
}
