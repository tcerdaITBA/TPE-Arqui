#include "commands.h"
#include "stdio.h"
#include "strings.h"
#include "fractals.h"
#include "time.h"
#include "paint.h"
#include "ctype.h"
#include "syscalls.h"
#include "stdlib.h"

#define CMDS_SIZE (sizeof(commands)/sizeof(commands[0]))

#define SLEEP_TIME 5000

static int help(const char *args);
static int fractals(const char *args);
static int clear(const char *args);
static int getTime(const char *args);
static int echo(const char *args);
static void printWithTimeFormat(unsigned int n);
static int set_GMT(const char *args);

typedef struct {
	const char * name;
	int (*function) (const char *str);
} command;

/* COMMANDS ARRAY */
static command commands[]= {{"help", help},
							{"set_GMT", set_GMT},
							{"fractals", fractals},
							{"time", getTime},
							{"clear", clear},
							{"echo", echo},
							};

/* EXECUTE */

/*
** Ejecuta la funcion name pasandole como parametro el string args.
** Se asegura desde la shell que toda palabra se encuentra separada
** por un unico espacio y que no existen espacios al comienzo o al
** de la cadena.
** Se asegura tambien que args[0] es un espacio en blanco (' ') si
** se escribieron argumentos o caracter nulo ('\0') sino.
** La funciones son responsables de parsear los argumentos, retornando
** INVALID_ARGS en caso de que estos sean incorrectos.
*/
int execute(const char *name, const char *args) {
	int i;
	for (i = 0; i < CMDS_SIZE; i++) {
		if (strcmp(name, commands[i].name) == 0)
			return (* commands[i].function) (args);
	}
	return UNSUPPORTED;
}

/* COMANDOS */

static int help(const char *args){
  if (args[0] != '\0')
	return INVALID_ARGS;
  printf("HELP FUNCTION -- shows the principal User Commands and its description\n\n");
  printf(" echo [args...]");
  printf("    Write arguments to the standard output. Display the args, separated by a single space character\n");
  printf("                   and followed by a newline, on the standard output.\n");

  printf(" clear                Clear the terminal screen.\n");
  printf(" time                 Display the current time on the standard output using 24hr format [hh:mm:ss]\n");
  printf(" set_GMT [GMT]        Set new Greenwich Mean Time. Displays new current time afterwards\n");
  printf(" fractals [*option]   Display a new fractal drawing on the standard output.\n");
  printf("                      If a specific fractal is desired, a number from 1 to %d may be sent as parameter.\n", fractals_size());
  printf("                      If no parameter is sent a random fractal will be drawed.\n\n");
  return VALID;
}

static int set_GMT (const char *args) {
  if (args[0] == '\0' || (args[1] != '-' && !isdigit(args[1])))
  	return INVALID_ARGS;
  int valid = setGMT(atoi(args));
  if (!valid)
  	return INVALID_ARGS;
  getTime("");
  return VALID;
}

static int getTime(const char *args) {
  int h = hour(), m = minutes(), s = seconds();
  printf("Current time: ");
  printWithTimeFormat(h);
  printf(":");
  printWithTimeFormat(m);
  printf(":");
  printWithTimeFormat(s);
  printf("\n");
  return VALID;
}

static void printWithTimeFormat(unsigned int n) {
    if (n < 10)
        printf("0");
    printf("%d", n);
}

static int clear(const char *args) {
  int rows = text_rows();
  for (int i = 0; i < 2*rows; i++)
    putchar('\n');
  putchar('\r'); // Vuelve el cursor al inicio de la pantalla
  return VALID;
}

static int echo(const char *args) {
  int spaced = args[0] == ' ' ;
  if (spaced || args[0] == '\0')
    printf("%s\n", args+spaced);
  return VALID;
}

static int fractals(const char *args) {
  int index = -1;
  int valid = 0;

  if (args[0] == '\0') // No se enviaron parametros --> fractal al azar
    index = seconds() % fractals_size();
  else if (isdigit(args[1]))
    index = atoi(args)-1;

  if (index != -1)
    valid = draw_fractal(index);

  if (!valid)
    return INVALID_ARGS;

  sleep(SLEEP_TIME);
  clear("");
  return VALID;
}
