#ifndef _PROC_MANAGER_H_
#define _PROC_MANAGER_H_

#include <stdint.h>

#define MAX_PROCESSES 256

uint64_t next_process(uint64_t current_rsp);
void exec_process(uint64_t new_rip);
void end_process();
void yield_process();
int number_processes();

#endif