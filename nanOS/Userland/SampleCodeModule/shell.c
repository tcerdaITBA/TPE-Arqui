#include "stdio.h"
#include "strings.h"
#include "fractals.h"
#include "time.h"
#include "shell.h"
#include "paint.h"

static void help();
static void fractals();
static void clear();
static void getTime();
static void echo(const char * str);
static void printWithTimeFormat(unsigned int n);
static void set_GMT(const char *str);

#define UNSUPPORTED_COMMAND "Unsupported Command\n"
#define INVALID_ARGUMENTS "Invalid Arguments\n"

struct fractalParams{
  double zoom;
  double moveX;
  double moveY;
  unsigned int maxIterations;
  double cRe;
  double cIm;
} param[6] = {
          {1265.76110029, 0.427834925, 0.42325986, 128, -0.6548832, -0.4477065},
          {0.85035, 0.437267, -0.613638, 120, -0.47098, -0.68545},
          {0.83035, 0.087267, -0.013638, 128, -0.77098, -0.08545},
          {1265.7611, 0.20932, 0.10459, 256, -0.6448832, -0.447706},
          {0.830350, 0.06595, -0.122026, 128, -0.654883, -0.4477065},
          {1265.76110029, 0.2093925, 0.1045986, 128, -0.6548832, -0.4477065}
        };


#define MAX_CMD_SIZE 256
#define CMD_SAVED 16
#define FRACTALS_QTY 6

int shell() {
    int run = 1;
    char buffer[MAX_CMD_SIZE];
    int len = MAX_CMD_SIZE;

    while (run) {
        printf("> ");
        readline(buffer, len);
        if (buffer[0] != '\0') // Se escribio algo
            execute(buffer);
    }

    return 0;
}


void execute(char * cmd) {

    if (strcmp(cmd, "fractals") == 0)
      fractals();
    else if (strcmp(cmd, "help") == 0)
      help();
    else if (strcmp(cmd, "clear") == 0)
      clear();
    else if (strcmp(cmd, "time") == 0)
      getTime();
    else if (strncmp(cmd, "echo", 4) == 0)
      echo(cmd + 4);
    else if (strncmp(cmd, "set GMT ", 8) == 0)
      set_GMT(cmd + 8);
    else
      printf(UNSUPPORTED_COMMAND);
}

static void help(){
  printf("HELP FUNCTION -- shows the principal User Commands and its description\n\n");
  printf(" echo [args...]");
  printf("    Write arguments to the standard output. Display the args, separated by a single space character\n");
  printf("                   and followed by a newline, on the standard output.\n");

  printf(" clear             Clear the terminal screen.\n");
  printf(" time              Display the current time on the standard output using 24hr format [hh:mm:ss]\n");
  printf(" set GMT [GMT]     Set new Greenwich Mean Time. Displays new current time afterwards\n");
  printf(" fractals          Display a new fractal drawing on the standard output\n\n");
}

static void set_GMT (const char *str) {
  int valid = setGMT(atoi(str));
  if (valid)
    getTime();
  else
    printf(INVALID_ARGUMENTS);
}

static void fractals() {
  int index = seconds() % FRACTALS_QTY; //fractal al azar
  drawJuliaFractal(1024, 768, param[index].zoom, param[index].moveX, param[index].moveY, param[index].maxIterations, param[index].cRe, param[index].cIm);

  sleep(5000);
  //clear();
}

static void getTime() {
  int h = hour(), m = minutes(), s = seconds();
  printf("Current time: ");
  printWithTimeFormat(h);
  printf(":");
  printWithTimeFormat(m);
  printf(":");
  printWithTimeFormat(s);
  printf("\n");
}

static void printWithTimeFormat(unsigned int n) {
    if (n < 10)
        printf("0");
    printf("%d", n);
}

static void clear() {
  int rows = 48; // TODO hacer con el syscall de cantidad de filas de texto
  for (int i = 0; i < 2*rows; i++)
    printf("\n");

  putchar('\t'); // Vuelve el cursor al inicio de la pantalla
}

static void echo(const char * str) {
  int spaced = str[0] == ' ' ;
  if (spaced || str[0] == '\0')
    printf("%s\n", str+spaced);
  else
    printf(UNSUPPORTED_COMMAND);
}
