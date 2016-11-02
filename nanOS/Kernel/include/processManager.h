#ifndef _PROC_MANAGER_H_
#define _PROC_MANAGER_H_

#include <stdint.h>

#define MAX_PROCESSES 256

typedef char status;

struct c_process {
	status st;
	uint64_t entry_point;
	uint64_t rsp;
	uint64_t stack_page;
	uint64_t pid;
	uint64_t ppid;
};

typedef struct c_process process;

uint64_t next_process(uint64_t current_rsp);
void exec_process(uint64_t new_rip, uint64_t params);
void end_process();
void yield_process();
uint64_t number_processes();
process * get_current_process();

#endif
