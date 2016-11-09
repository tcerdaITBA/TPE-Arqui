#include "fifo.h"
#include "process.h"
#include "processManager.h"
#include "kernelMutex.h"
#include "queueADT.h"
#include "strings.h"
#include "lib.h"
#include "memoryAllocator.h"
#include "videoDriver.h"

typedef struct {
  char buffer[BUF_SIZE];
  int free_slot;
  int current;
  int buf_fill;
} circular_buffer;

typedef struct {
  char name[FIFO_NAME_LEN];
  circular_buffer c_buffer;
  int state;
  int fifo_mutex_key;
  queueADT read_queue;
} fifo;

typedef struct {
  int bytes;
  int bytes_read;
  void * buffer;
  fifo * f;
  process * reader_p;
} read_request;

static fifo open_fifos[MAX_FIFOS];

static int try_read(read_request * r);
static fifo create_new_fifo(char * name);
static void send_to_readers(queueADT read_queue);
static read_request * create_read_request(int fifo_key, void * buf, int bytes);
static int is_open(int key);
static void release_readers(queueADT q);

static circular_buffer create_circular_buffer();
static int write_circular_buffer(circular_buffer * c_buf, const void * source, int bytes);
static int read_circular_buffer(circular_buffer * c_buf, void * dest, int bytes);


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
  f.state = OPEN;
  f.c_buffer = create_circular_buffer();
  f.fifo_mutex_key = mutex_open(name); // TODO: Ver que nombre pasarle.
  f.read_queue = create_queue();
  return f;
}

static circular_buffer create_circular_buffer() {
  circular_buffer b;

  b.buf_fill = 0;
  b.current = 0;
  b.free_slot = 0;

  return b;
}

static read_request * create_read_request(int fifo_key, void * buf, int bytes) {
  read_request * r = (read_request *) get_page(sizeof((*r)));
  r->bytes = bytes;
  r->buffer = buf;
  r->f = &open_fifos[fifo_key];
  r->reader_p = get_current_process();
  return r;
}

static int is_open(int key) {
  return key < MAX_FIFOS && open_fifos[key].state == OPEN;
}

int fifo_write(int key, const void * buf, int bytes) {
  int write_bytes;
  fifo * f;

  if (is_open(key) && bytes > 0) {
    f = &open_fifos[key];
    mutex_lock(f->fifo_mutex_key);

    write_bytes = write_circular_buffer(&f->c_buffer, buf, bytes);

    send_to_readers(f->read_queue);

    mutex_unlock(f->fifo_mutex_key);
    return write_bytes;
  }
  else if (bytes <= 0) {
    return 0;
  }

  return FIFO_NOT_OPEN_ERROR;
}

static void send_to_readers(queueADT read_queue) {
  print_str("|SENT TO READERS|\n", 10, 40);
  if (!is_empty(read_queue)) {
    print_str("|Peeking readers|\n", 10, 40);
    while(try_read(peek(read_queue))) {
      // manda a los lectores hasta que uno no pueda leer mas
      read_request * r = dequeue(read_queue);
      print_str("|Unblocking process|\n", 10, 60);
      unblock_process(r->reader_p);// leyo, entonces lo saca de los que estan esperando leer
    }
  }
}

int fifo_read(int key, void * buf, int bytes) {
  int could_read = 0;
  int n;

  if (is_open(key) && bytes > 0) {
    fifo * f = &open_fifos[key];

    read_request * r = create_read_request(key, buf, bytes);

    mutex_lock(f->fifo_mutex_key);

    if (is_empty(f->read_queue)) // primer reader
      could_read = try_read(r);
    else {  // mas de un reader esperando
      enqueue(f->read_queue, r);
    }

    if (!could_read) {
      print_str("| COULDN'T READ |\n", 13, 60);
      enqueue(f->read_queue, r);
      mutex_unlock(f->fifo_mutex_key);
      print_str("| UNLOCKED |\n", 15, 60);
      block_process(r->reader_p);
      yield_process(); // esta bloqueado
    }
    else {
      mutex_unlock(f->fifo_mutex_key); // TODO: OJO que el try_read bloquea, puede que nunca se llegue a este unlock
    }

    if (!is_open(key)) // se desbloqueo porque cerraron el fifo;
      return FIFO_NOT_OPEN_ERROR;

    n = r->bytes_read;

    store_page((uint64_t) r);
    return n;
  }
  else {
    return 0;
  }

  return FIFO_NOT_OPEN_ERROR;
}

int fifo_close(int key) {
  if (is_open(key)) {
    fifo * f = &open_fifos[key];
    release_readers(f->read_queue);
    destroy_queue(f->read_queue);
    f->state = CLOSED;
    return 1;
  }
  return FIFO_NOT_OPEN_ERROR;
}

static void release_readers(queueADT q) {
  read_request * req;
  while (!is_empty(q)) {
    req = dequeue(q);
    unblock_process(req->reader_p);
  }
}

static int try_read(read_request * r) {
  if (r->f->c_buffer.buf_fill > 0) {  // el buffer tenga los bytes que quiero leer
    r->bytes_read = read_circular_buffer(&r->f->c_buffer, r->buffer, r->bytes);
    //unblock_process(r->reader_p);
    return 1; // lee
  }
  else {
    //block_process(r->reader_p);
    return 0; // no lee
  }
}

static int write_circular_buffer(circular_buffer * c_buf, const void * source, int bytes) {
  int write_bytes = (c_buf->buf_fill + bytes) > BUF_SIZE ? BUF_SIZE - c_buf->buf_fill : bytes;
  int aux = c_buf->free_slot;

  c_buf->free_slot = (c_buf->free_slot + write_bytes) % BUF_SIZE;

  if (aux < c_buf->free_slot) {
    memcpy(c_buf->buffer+aux, source, write_bytes);
  }
  else {
    int first_bytes = BUF_SIZE - aux;
    int second_bytes = write_bytes - first_bytes;

    memcpy(c_buf->buffer+aux, source, first_bytes); /* Copia hasta el final del buffer */
    memcpy(c_buf->buffer, source+first_bytes, second_bytes); /* Copia a partir del comienzo del buffer lo que faltaba de source */
  }

  c_buf->buf_fill += write_bytes;
  return write_bytes;
}

static int read_circular_buffer(circular_buffer * c_buf, void * dest, int bytes) {
  int read_bytes = c_buf->buf_fill < bytes ? c_buf->buf_fill : bytes;
  int aux = c_buf->current;

  c_buf->current = (c_buf->current + read_bytes) % BUF_SIZE;

  if (aux < c_buf->current) {
    memcpy(dest, c_buf->buffer + aux, read_bytes);
  }
  else {
    int first_bytes = BUF_SIZE - aux;
    int second_bytes = read_bytes - first_bytes;

    memcpy(dest, c_buf->buffer + aux, first_bytes);
    memcpy(dest+first_bytes, c_buf->buffer, second_bytes);
  }

  c_buf->buf_fill -= read_bytes;

  return read_bytes;
}
