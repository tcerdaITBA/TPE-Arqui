#include "syscalls.h"
#include "strings.h"
#include "stdlib.h"
#include "stdio.h"

#define MAX_ARGS 32

typedef int (*entry_point) (int, char **);

static void executer(void ** params);
static void build_arguments(const char * arg, char * arg_strings, char * arguments[]);
static int tester(int argc, const char * argv[]);

int execp(void * function_ptr, const char * arg) {
	void * memory = malloc(strlen(arg) + (MAX_ARGS * sizeof(char *)));
	char ** arguments = memory;
	char * arg_strings = memory + MAX_ARGS * sizeof(char *);

	arguments[0] = (void *) tester;

	build_arguments(arg, arg_strings, arguments + 1);

	return exec(executer, (uint64_t) memory);
}

static void executer(void ** params) {
	entry_point function = params[0];
	char ** argv = (char **) params + 1;
	int argc = 0;
	int ret_value;

	while (argv[argc] != NULL)
		argc++;

	ret_value = (*function)(argc, argv);

	free(params);
	set_foreground(ppid());
	end();
}

static int tester(int argc, const char * argv[]) {
	int i;

	for(i = 0; i < argc; i++)
		printf("%s\n", argv[i]);
		
	return argc;
}

static void build_arguments(const char * arg, char * arg_strings, char * arguments[]) {
	int i = 0;
	int j;
	char * ptr;

	if (arg[0] == '\0') {
		arguments[0] = NULL;
		return;
	}

	ptr = arg_strings;

	for (j = 0; arg[j] != '\0'; j++) {
		if (arg[j] == ' ') {
			arg_strings[j] = '\0';
			arguments[i++] = ptr;
			ptr = arg_strings + j + 1;
		}
		else
			arg_strings[j] = arg[j];
	}

	arg_strings[j] = '\0';
	arguments[i++] = ptr;
	arguments[i] = NULL;
}
