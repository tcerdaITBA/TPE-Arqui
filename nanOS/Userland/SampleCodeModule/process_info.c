#include "process_info.h"
#include "syscalls.h"
#include "strings.h"
#include "stdlib.h"
#include "stdio.h"

#define MAX_COMMAND 256

#define SEPARATOR ' '
#define FOREGROUND "foreground"
#define BACKGROUND "background"
#define STACK_ADDR "Stack Address: "
#define MEM_ADDR "Memory Address: "
#define PPID "PPID"

typedef struct int_node_c {
	int pid;
	struct int_node_c * next;
} int_node;

static char * state[] = {"RUNNING", "READY", "BLOCKED"};

static int_node * first = NULL;

static void add_pid_command(int pid);
static void remove_pid_command(int pid);
static void print_processes_command();
static void extract_commands(char * str, char * commands[]);
static int strcpysep (char * to, const char * source, char separator);
static int has_mem_pages(const process_info * p);
static void send_num(int pid, char comm);

static void add_pid_command(int pid) {
	int_node * n = malloc(sizeof(*n));

	n->pid = pid;
	n->next = first;

	first = n;
}

static void remove_pid_command(int pid) {
	int_node * current = first;
	int_node * prev = NULL;

	while (current != NULL) {
		if (current->pid == pid) {

			if (prev != NULL)
				prev->next = current->next;
			else
				first = current->next;
			free(current);
			return;
		}

		prev = current;
		current = current->next;
	}
}

static void print_processes_command() {
	char buffer [MAX_PROCESS_STRING];
	int_node * current = first;
	process_info pi;

	printf("PROCESSES:\n");

	while (current != NULL) {
		if (get_process_info(current->pid, &pi)) {
			process_string(&pi, buffer);
			printf("%s\n", buffer);
		}
		current = current->next;
	}
}

static void execute_command(char * command) {
	char c = command[0];

	switch(c) {
		case ADD_PID:
			add_pid_command(atoi(command+1));
			break;
		case REMOVE_PID:
			remove_pid_command(atoi(command+1));
			break;
		case PRINT_PROCESSES:
			print_processes_command();
			break;
	}
}

static void extract_commands(char * str, char * commands[]) {
	int i, j;
	char * ptr = str;

	for (i = j = 0; str[i] != '\0'; i++) {
		if (str[i] == PROCESS_INFO_COMMAND_SEPARATOR) {
			str[i] = '\0';
			commands[j++] = ptr;
			ptr = str + i + 1;
		}
	}

	commands[j] = NULL;
}

void process_info_manager() {
	int fds = fifo_open(PROCESS_INFO_FIFO);
	char command[MAX_COMMAND];
	char * commands[MAX_COMMAND];
	int len, i;

	add_pid_command(0);
	add_pid_command(1);

	while (1) {
		len = read(fds, command, MAX_COMMAND);
		command[len] = '\0';
		extract_commands(command, commands);
		for (i = 0; commands[i] != NULL; i++) {
			execute_command(commands[i]);
		}
	}
}

int process_string(const process_info * p, char buffer[MAX_PROCESS_STRING]) {
	int i = 0;
	char * ground = p->is_foreground ? FOREGROUND : BACKGROUND;

	i += itoa(p->pid, buffer + i, 10);

	buffer[i++] = SEPARATOR;

	i += strcpysep(buffer + i, p->name, SEPARATOR); /* nombre */

	i += strcpysep(buffer + i, PPID, SEPARATOR);

	i += itoa(p->ppid, buffer + i, 10);

	buffer[i++] = SEPARATOR;

	i += strcpysep(buffer + i, ground, SEPARATOR); /* [background|foregound] */

	i += strcpysep(buffer + i, state[(int)p->st], SEPARATOR); /* [RUNNING|READY|BLOCKED]*/

	i += strcpysep(buffer + i, STACK_ADDR, SEPARATOR);

	i += itoa((uint64_t) p->stack_address, buffer + i, 16);

	buffer[i++] = SEPARATOR;

	if (has_mem_pages(p)) {
		int j;

		i += strcpysep(buffer + i, MEM_ADDR, SEPARATOR);

		for (j = 0; p->mem_pages[j] != NULL; j++) {
			i += itoa((uint64_t)p->mem_pages[j], buffer + i, 16);
			buffer[i++] = ',';
			buffer[i++] = ' ';
		}
	}

	buffer[i] = '\0';

	return i;
}

static int strcpysep (char * to, const char * source, char separator) {
	int len = strcpy(to, source);
	to[len++] = separator;
	to[len] = '\0';

	return len;
}

static int has_mem_pages(const process_info * p) {
	return p->mem_pages[0] != NULL;
}

static void send_num(int pid, char comm) {
	int fds = fifo_open(PROCESS_INFO_FIFO);
	char c = comm;
	char command[MAX_COMMAND];
	int digits;

	command[0] = c;
	digits = itoa(pid, command+1, 10);
	command[digits+1] = PROCESS_INFO_COMMAND_SEPARATOR;

	write(fds, command, digits+2);
}

void add_pid(int pid) {
	send_num(pid, ADD_PID);
}

void remove_pid(int pid) {
	send_num(pid, REMOVE_PID);
}

void print_processes() {
	int fds = fifo_open(PROCESS_INFO_FIFO);
	char c = PRINT_PROCESSES;
	char command[2];

	command[0] = c;
	command[1] = PROCESS_INFO_COMMAND_SEPARATOR;

	write(fds, command, 2);
}