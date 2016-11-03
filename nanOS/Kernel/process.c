
#include "process.h"
#include "memoryAllocator.h"
#include "processManager.h"

#define NULL ((void *) 0)


/* El stack frame y el llenado del mismo se tomó de
** https://bitbucket.org/RowDaBoat/wyrm
*/
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


struct c_process {
	status st;
	uint64_t entry_point;
	uint64_t rsp;
	uint64_t stack_page;
	uint64_t pid;
	uint64_t ppid;
};


/* Próximo pid a asignar */
static uint64_t next_pid = 0;

static process * foreground = NULL;

static uint64_t fill_stack(uint64_t rip, uint64_t rsp, uint64_t params);

process * create_process(uint64_t new_process_rip, uint64_t params) {

	process * new_process = (process *) get_page(sizeof(* new_process));

	new_process->stack_page = get_stack_page(); /* Pide al MemoryAllocator espacio para el stack */

	new_process->entry_point = new_process_rip;

	new_process->st = READY;

	new_process->rsp = fill_stack(new_process_rip, new_process->stack_page, params);

	if (next_pid != 0) /* No es el primer proceso */
		new_process->ppid = pid_process(get_current_process());

	new_process->pid = next_pid++;

	return new_process;
}

void destroy_process(process * p) {
	store_stack_page(p->stack_page);
	store_page((uint64_t) p);
}

void set_rsp_process(process * p, uint64_t rsp) {
	p->rsp = rsp;
}

uint64_t get_rsp_process(process * p) {
	return p->rsp;
}

uint64_t pid_process(process * p) {
	return p->pid;
}

uint64_t ppid_process(process * p) {
	return p->ppid;
}

void block_process(process * p) {
	p->st = BLOCKED;
}

void unblock_process(process * p) {
	p->st = READY;
}

int is_blocked_process(process * p) {
	return p->st != READY;
}

void unblock_read_process(process * p) {
	if (p->st == BLOCKED_READ)
		unblock_process(p);
}

void block_read_process(process * p) {
	p->st = BLOCKED_READ;
}

void set_foreground_process (process *p) {
	if (foreground == get_current_process()) {
		foreground = p;
		unblock_process(p);
	}
}

process * get_foreground_process() {
	return foreground;
}

/* Llena el stack para que sea hookeado al cargar un nuevo proceso
** https://bitbucket.org/RowDaBoat/wyrm */
static uint64_t fill_stack(uint64_t rip, uint64_t stack_page, uint64_t params) {
	StackFrame * frame = (StackFrame *) stack_page - 1;

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
