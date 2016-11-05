#ifndef _PROC_MANAGER_H_
#define _PROC_MANAGER_H_

#include <stdint.h>
#include "process.h"

#define MAX_PROCESSES 256

uint64_t next_process(uint64_t current_rsp);

void exec_process(uint64_t new_rip, uint64_t params);
void end_process();
void yield_process();

uint64_t number_processes();

process * get_current_process();

void assign_quantum();
void unassign_quantum();

#endif
