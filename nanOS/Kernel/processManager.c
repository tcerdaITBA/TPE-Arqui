#include "memoryAllocator.h"
#include "processManager.h"
#include "dirs.h"
#include "videoDriver.h"
#include "process.h"


#define NULL ((void *) 0)

#define UNLOCKED 1
#define LOCKED 0

typedef struct c_node {
	process *p;
	struct c_node *next;
} list_node;


void _hlt();

static void add_process(process *p);
void _change_process(uint64_t rsp);

static int64_t process_lock = UNLOCKED;

extern int _unlocked(int64_t * locknum);


/* Proceso actualmente corriendo */
static list_node *current = NULL;
static list_node *prev = NULL;

static uint64_t n_processes = 0;

void _yield_process();


static void lock_list() {
	while (!_unlocked(&process_lock))
		yield_process();
}

static void unlock_list() {
	process_lock = UNLOCKED;
}

process * get_current_process() {
	return current->p;
}

uint64_t next_process(uint64_t current_rsp) {
	if (current == NULL)
		return current_rsp;

	set_rsp_process(current->p, current_rsp);

	prev = current;
	current = current->next;

	while (is_blocked_process(current->p)) {
		prev = current;
		current = current->next;
	}

	
	return get_rsp_process(current->p);
}

void exec_process(uint64_t new_process_rip, uint64_t params) {
	process * new_process = create_process(new_process_rip, params);

	add_process(new_process);

	if (pid_process(new_process) == 0)
		_change_process(get_rsp_process(current->p));
}

static void add_process(process * p) {

	lock_list();

	list_node *new_node = (list_node *) get_page(sizeof(*new_node));

	new_node->p = p;

	if (current == NULL) {
		current = new_node;
		current->next = current;
		prev = current;
	}
	else {
		new_node->next = current->next;
		current->next = new_node;
	}

	n_processes++;

	unlock_list();
}

void yield_process() {
	_hlt();
}

/* Se avanza con el proceso que está delante */
void end_process() {

	lock_list();

	list_node * n = current;

	prev->next = current->next;
	current = current->next;
	n_processes--;

	destroy_process(n->p);
	store_page((uint64_t) n);

	unlock_list();


	_change_process(get_rsp_process(current->p));
}

uint64_t number_processes() {
	return n_processes;
}
