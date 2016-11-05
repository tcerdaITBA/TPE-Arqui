#include "commands.h"
#include "stdio.h"
#include "strings.h"
#include "fractals.h"
#include "time.h"
#include "paint.h"
#include "ctype.h"
#include "syscalls.h"
#include "stdlib.h"
#include "graphics.h"

#include <stdint.h>

/* Longitud maxima de un comando ingresado por el usuario */
#define CMDS_SIZE (sizeof(commands)/sizeof(commands[0]))

#define SLEEP_TIME 5000

static int help(const char *args);
static int fractals(const char *args);
static int clear(const char *args);
static int getTime(const char *args);
static int echo(const char *args);
static void printWithTimeFormat(unsigned int n);
static int set_GMT(const char *args);
static int isnum(const char *str);
static const char * next_arg (const char *args);
static int change_char_color (const char *args);
static int extract_colors (const char *args, int *r, int *g, int *b);
static int change_bg_color (const char *args);
static void fractal_process(int index);

static void test(uint64_t param);
static int test_mt(const char *str);

/* Estructura que representa un comando de la Shell */
typedef struct {
	const char * name;  /* Nombre del comando */
	int (*function) (const char *str);  /* Funcion correspondiente al comando */
} command;

/* COMMANDS ARRAY */
static command commands[]= {{"help", help},
							{"set_GMT", set_GMT},
							{"fractals", fractals},
							{"time", getTime},
							{"clear", clear},
							{"echo", echo},
              {"char_color", change_char_color},
              {"bg_color", change_bg_color},
              {"test", test_mt}
							};

static int test_num = 0;
static void test(uint64_t param) {
  int i = 25;
  int tn = test_num++;

  int m_key = mutex_open("test_mutex");

  mutex_lock(m_key);
  printf("Tengo el lock. Soy %d\n", tn);

//  while (i--) {
 //   printf("%d ", i);
    sleep(3000);
//    yield();
//  }

  //draw_fractal(param);

  printf("Soltando lock. Soy %d\n", tn);

  mutex_unlock(m_key);

  end();
}

static int test_mt(const char *str) {
//  int i = 100;

//  while (--i)
  printf("PID = %d", exec((void *) test, atoi(str)));
  return VALID;
}


/* EXECUTE */

/*
** Ejecuta la funcion name pasandole como parametro el string args.
** Se asegura desde la shell que toda palabra se encuentra separada
** por un unico espacio y que no existen espacios al comienzo o al
** de la cadena.
** Se asegura tambien que args[0] es el caracter nulo ('\0') si no se 
** enviaron argumentos o el comienzo del primer argumento sino.
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

/* Muestra en pantalla texto de ayuda al usuario, por ejemplo comandos existentes */
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
  printf("                      If no parameter is sent a random fractal will be drawed.\n");
  printf(" char_color [r,g,b]   Changes character color.\n");
  printf(" bg_color   [r,g,b]   Changes background color. Clears screen.\n\n");
  return VALID;
}

/* Setea GMT del reloj y muestra la hora actual en pantalla*/
static int set_GMT (const char *args) {
  if (args[0] == '\0' || !isnum(args) || (*next_arg(args)) != '\0') // Se envió un argumento no entero o más de un argumento
  	return INVALID_ARGS;
  int valid = setGMT(atoi(args));
  if (!valid)
  	return INVALID_ARGS;
  getTime("");
  return VALID;
}

/* Retorna 1 si una cadena de caracteres representa a un numero */
static int isnum(const char *str) {
  if (!isdigit(str[0]) && str[0] != '-')
    return 0;
  if (str[0] == '-' && !isdigit(str[1]))
    return 0;
  int sign = str[0] == '-';
  int i;
  for (i = sign; str[i] != '\0' && str[i] != ' '; i++)
    if (!isdigit(str[i]))
      return 0;
  return 1;
}

/*Imprime en pantalla la hora actual */
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

/* Imprime un numero en formato [xx]*/
static void printWithTimeFormat(unsigned int n) {
    if (n < 10)
        printf("0");
    printf("%d", n);
}

/* Limpia la pantalla de la terminal*/
static int clear(const char *args) {
  int rows = text_rows();
  for (int i = 0; i < 2*rows; i++)
    putchar('\n');
  putchar('\r'); // Vuelve el cursor al inicio de la pantalla
  return VALID;
}

/*Imprime la cadena de argumentos pasadas como parametros a salida estandar */
static int echo(const char *args) {
  printf("%s\n", args);
  return VALID;
}

/*
** Muestra en pantalla imagenes de fractales.
** En caso de recibir un numero como parámetro muestra el fractal correspondiente,
** si no se recibió ningun parametro muestra un fractal al azar
*/
static void fractal_process(int index) {
  draw_fractal(index);
  end();
}

static int fractals(const char *args) {
  int index = -1;

  if (args[0] == '\0') // No se enviaron parametros --> fractal al azar
    index = seconds() % fractals_size();
  else if (isnum(args) && (*next_arg(args)) == '\0') // se envió un solo parámetro y es un número
    index = atoi(args)-1;

  if (index < 0 || index >= fractals_size())
    return INVALID_ARGS;
  
  exec((void *) fractal_process, index);

  return VALID;
}

/* Cambia el color de la letra */
static int change_char_color (const char *args) {
  int red, green, blue;
  if(!extract_colors(args, &red, &green, &blue))
    return INVALID_ARGS;
  return set_char_color(red, green, blue) == 0 ? INVALID_ARGS : VALID;
}

/* Cambia eñ color del fondo */
static int change_bg_color (const char *args) {
  int red, green, blue;
  if(!extract_colors(args, &red, &green, &blue))
    return INVALID_ARGS;
  if(set_bg_color(red, green, blue)) {
    clear("");
    return VALID;
  }
  return INVALID_ARGS;
}

/* Retorna el siguiente argumento , es decir lo siguiente luego de un espacio */
static const char * next_arg (const char *args) {
  int i = 0;
  while (args[i] != ' ' && args[i] != '\0')
    i++;
  return (args[i] == ' ') ? args+i+1 : args+i;
}

/* Obtiene a partir de lo ingresado por el usuario, los valores de colores RGB */
static int extract_colors (const char *args, int *r, int *g, int *b) {
  const char *red = args;
  args = next_arg(args);
  const char *green = args;
  const char *blue = next_arg(args);
  if (!isnum(red) || !isnum (green) || !isnum (blue))
    return 0;
  *r = atoi(red);
  *g = atoi(green);
  *b = atoi(blue);
  return 1;
}