#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>

#include "defs.h"

#define READY 1
#define BLOCKED 0
#define BLOCKED_READ 2

typedef struct c_process process;
typedef char status;

process * create_process(uint64_t rip, uint64_t params);
void destroy_process(process * p);

void set_rsp_process(process * p, uint64_t rsp);
uint64_t get_rsp_process(process * p);

void block_process(process * p);
void unblock_process(process * p);
int is_blocked_process(process * p);
void unblock_read_process(process * p);
void block_read_process(process * p);

uint64_t pid_process(process * p);
uint64_t ppid_process(process * p);

process * get_process_by_pid (uint64_t pid);

/* Quizas no tengan que estar aca */
void set_foreground_process (process *p);
process * get_foreground_process();

/* Archivos del proceso */
int set_file_open(process * p, int fd);
int set_file_closed(process * p, int fd);
int file_is_open(process * p, int fd);

#endif
