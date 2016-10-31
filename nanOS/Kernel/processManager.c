#include "memoryAllocator.h"
#include "processManager.h"
#include "dirs.h"
#include "videoDriver.h"

/* El stack frame y el llenado del mismo se tomó de 
** https://bitbucket.org/RowDaBoat/wyrm
*/


typedef struct {
	uint64_t rsp;
	uint64_t stack_page;
	uint64_t pid;
	uint64_t ppid;
} process;

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

/* Funciona como una cola */
static process process_table[MAX_PROCESSES];

/* Índice del proceso actualmente corriendo */
static int current_index = -1;

/* Índice del lugar libre en la cola */
static int free_slot = 0;

/* Próximo pid a asignar */
static uint64_t next_pid = 1;

uint64_t next_process(uint64_t current_rsp) {
	process current = process_table[current_index++];

	current.rsp = current_rsp; /* Se guarda el stack pointer actualizado */

	add_process(current); /* Coloca proceso al final de la cola */

	print_num(current.pid, 5, 5);
	if (current_index == MAX_PROCESSES)
		current_index = 0;

	return process_table[current_index].rsp;
}

void exec_process(uint64_t new_process_rip, uint64_t params) {
	process new_process;
	new_process.stack_page = get_stack_page(); /* Pide al MemoryAllocator espacio para el stack */

	new_process.rsp = fill_stack(new_process_rip, new_process.stack_page, params);

	if (number_processes > 0) /* No es el primer proceso */
		new_process.ppid = process_table[current_index].pid;

	new_process.pid = next_pid++;

	add_process(new_process);
}

static void add_process(process p) {
	process_table[free_slot++] = p;

	if (free_slot == MAX_PROCESSES)
		free_slot = 0;
}

void _yield_process();
void _change_process(uint64_t rsp);

void yield_process() {
	_yield_process();
}

/* Se avanza con el proceso que está delante */
void end_process() {
	store_stack_page(process_table[current_index].stack_page);
	_change_process(process_table[++current_index].rsp);
}

int number_processes() {
	return current_index <= free_slot ? free_slot - current_index : MAX_PROCESSES - (current_index - free_slot);
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