#include "process_info.h"
#include "syscalls.h"
#include "strings.h"
#include "stdlib.h"

#define NULL ((void *) 0)

#define SEPARATOR ' '
#define FOREGROUND "foreground"
#define BACKGROUND "background"
#define STACK_ADDR "Stack Address: "
#define MEM_ADDR "Memory Address: "

#define RUNNING 0
#define READY 1
#define BLOCKED 2

static char * state[] = {"RUNNING", "READY", "BLOCKED"};

typedef struct process_info_c {
	char is_foreground;
	char st;
	uint64_t pid;
	char name [MAX_PROCESS_NAME];
	void * stack_page;
	void * mem_pages[MAX_MEM_PAGES];
} process_info;


void process_string(const process_info * p, char buffer[MAX_PROCESS_STRING]) {
	int i = 0;
	char * ground = is_foreground ? FOREGROUND : BACKGROUND;

	i += itoa(p->pid, buffer + i, 10);

	buffer[i++] = SEPARATOR;

	i += strcpysep(buffer + i, p->name, SEPARATOR); /* nombre */

	i += strcpysep(buffer + i, ground, SEPARATOR); /* [background|foregound] */

	i += strcpysep(buffer + i, state[p->st], SEPARATOR); /* [RUNNING|READY|BLOCKED]*/

	i += strcpysep(buffer + i, STACK_ADDR, SEPARATOR);

	if (has_mem_pages(p)) {
		int j;

		i += strcpysep(buffer + i, MEM_ADDR, SEPARATOR);

		for (j = 0; mem_pages[j] != NULL; j++) {
			i += itoa(mem_pages[j], buffer + i, 16);
			buffer[i++] = ',';
			buffer[i++] = ' ';
		}
	}

	buffer[i] = '\0';

	return i;
}

static void strcpysep (char * to, const char * source, char separator) {
	int len = strcpy(to, source);
	to[len++] = separator;
	to[len] = '\0';

	return len;
}

static int has_mem_pages(const process_info * p) {
	return p->mem_pages[0] != NULL;
}