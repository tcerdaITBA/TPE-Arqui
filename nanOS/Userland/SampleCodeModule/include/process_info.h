#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#define MAX_PROCESS_NAME 64
#define MAX_MEM_PAGES 128
#define MAX_PROCESS_STRING 512

typedef struct process_info_c {
	char is_foreground;
	char st;
	uint64_t pid;
	char name [MAX_PROCESS_NAME];
	void * stack_address;
	void * mem_pages[MAX_MEM_PAGES];
} process_info;

/* Hace un toString del proceso. Se pasa un puntero para que sea más eficiente. */
void process_string(const process_info * p, char buffer[MAX_PROCESS_STRING]);

#endif