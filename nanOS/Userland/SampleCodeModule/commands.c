#include "commands.h"
#include "stdio.h"
#include "strings.h"
#include "fractals.h"
#include "time.h"
#include "paint.h"
#include "ctype.h"

#define CMDS_SIZE (sizeof(commands)/sizeof(commands[0]))

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


// TODO: mover a fractals.c
#define FRACTALS_QTY (sizeof(param)/sizeof(param[0]))
struct fractalParams{
  double zoom;
  double moveX;
  double moveY;
  unsigned int maxIterations;
  double cRe;
  double cIm;
} param[] = {
          {1265.76110029, 0.427834925, 0.42325986, 128, -0.6548832, -0.4477065},
          {0.85035, 0.437267, -0.613638, 120, -0.47098, -0.68545},
          {0.83035, 0.087267, -0.013638, 128, -0.77098, -0.08545},
          {1265.7611, 0.20932, 0.10459, 256, -0.6448832, -0.447706},
          {0.830350, 0.06595, -0.122026, 128, -0.654883, -0.4477065},
          {1265.76110029, 0.2093925, 0.1045986, 128, -0.6548832, -0.4477065}
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

  printf(" clear             Clear the terminal screen.\n");
  printf(" time              Display the current time on the standard output using 24hr format [hh:mm:ss]\n");
  printf(" set_GMT [GMT]     Set new Greenwich Mean Time. Displays new current time afterwards\n");
  printf(" fractals          Display a new fractal drawing on the standard output\n\n");
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
  int rows = 48; // TODO hacer con el syscall de cantidad de filas de texto
  for (int i = 0; i < 2*rows; i++)
    printf("\n");
  putchar('\t'); // Vuelve el cursor al inicio de la pantalla
  return VALID;
}

static int echo(const char *args) {
  int spaced = args[0] == ' ' ;
  if (spaced || args[0] == '\0')
    printf("%s\n", args+spaced);
  return VALID;
}

// TODO: mover logica a fractals.c
static int fractals(const char *args) {
  int index = seconds() % FRACTALS_QTY; //fractal al azar
  drawJuliaFractal(param[index].zoom, param[index].moveX, param[index].moveY, param[index].maxIterations, param[index].cRe, param[index].cIm);
  sleep(5000); // TODO: meter en un define
  clear("");
  return VALID;
}
