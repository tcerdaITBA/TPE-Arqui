#include "memoryAllocator.h"
#include "processManager.h"
#include "dirs.h"
#include "videoDriver.h"

#define READY 1
#define BLOCKED 0

#define NULL ((void *) 0)


/* El stack frame y el llenado del mismo se tomó de 
=======
/* El stack frame y el llenado del mismo se tomó de
>>>>>>> master
** https://bitbucket.org/RowDaBoat/wyrm
*/

typedef char status;

typedef struct c_node {
	process p;
	struct c_node *next;
} node;

typedef struct {
	//Registers restore context
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
} StackFrame;


static uint64_t fill_stack(uint64_t rip, uint64_t rsp, uint64_t params);
static void add_process(process p);

/* Índice del proceso actualmente corriendo */
static node *current = NULL;
static node *prev = NULL;

/* Próximo pid a asignar */
static uint64_t next_pid = 0;

static void set_rsp(uint64_t rsp) {
	current->p.rsp = rsp;
}

static int is_blocked(process p) {
	return p.st != READY;
}

static uint64_t get_rsp(process p) {
	return p.rsp;
}

uint64_t next_process(uint64_t current_rsp) {
	if (current == NULL)
		return current_rsp;

	set_rsp(current_rsp);

	prev = current;
	current = current->next;

	while (is_blocked(current->p))
		current = current->next;
	
	return get_rsp(current->p);
}

void _change_process(uint64_t rsp);


void exec_process(uint64_t new_process_rip, uint64_t params) {
	process new_process;
	new_process.stack_page = get_stack_page(); /* Pide al MemoryAllocator espacio para el stack */

	new_process.entry_point = new_process_rip;

	new_process.st = READY;

	new_process.rsp = fill_stack(new_process_rip, new_process.stack_page, params);

//	if (number_processes() > 0) /* No es el primer proceso */
//		new_process.ppid = current->p.pid;

	new_process.pid = next_pid++;

	add_process(new_process);

	if (new_process.pid == 0)
		_change_process(get_rsp(current->p));

}

static void add_process(process p) {
	node *new_node = (node *) get_page(sizeof(node));

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
}

void _yield_process();

void yield_process() {
	_yield_process();
}

/* Se avanza con el proceso que está delante */
void end_process() {
	store_stack_page(current->p.stack_page);
	store_page(current);
	prev->next = current->next;
	current = current->next;
	_change_process(get_rsp(current->p));
}

// TODO
int number_processes() {
	return 1;
}

/* Llena el stack para que sea hookeado al cargar un nuevo proceso
** https://bitbucket.org/RowDaBoat/wyrm */
static uint64_t fill_stack(uint64_t rip, uint64_t stack_page, uint64_t params) {
	StackFrame * frame = (StackFrame *) stack_page - 1; /* TODO: preguntar por el -1 */

	frame->gs =		0x001;
	frame->fs =		0x002;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	0x00B;
	frame->rdi =	params;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	rip;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return (uint64_t) &frame->gs;
}
