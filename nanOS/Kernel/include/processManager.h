#ifndef _PROC_MANAGER_H_
#define _PROC_MANAGER_H_

#include <stdint.h>

#define MAX_PROCESSES 256

typedef struct {
	uint64_t rsp;
	uint64_t stack_page;
	uint64_t pid;
	uint64_t ppid;
} process;


uint64_t next_process(uint64_t current_rsp);
void exec_process(uint64_t new_rip, uint64_t params);
void end_process();
void yield_process();
int number_processes();

#endif
