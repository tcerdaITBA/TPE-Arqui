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
static void echo(char * str);
static void printWithTimeFormat(unsigned int n);

#define MAX_CMD_SIZE 256
#define CMD_SAVED 16

int shell() {
    int run = 1;
    char buffer[MAX_CMD_SIZE];
    int len = MAX_CMD_SIZE;

    while (run) {
        printf(":>> ");
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
    else if (strcmp(cmd, "clock") == 0)
      getTime();
    else if (strncmp(cmd, "echo", 4) == 0 && cmd[4] == ' ') {
        echo(cmd + 5);
    }

    else
      printf("Unsupported Command\n");
}

static void help(){
  printf("-- FUNCIONES DISPONIBLES --\n\n \
fractals - muestra en pantalla un fractal (Julia's Set)\n \
clock - imprime en pantalla la hora actual\n \
clear - limpia la pantalla y vuelve el cursor al inicio\n \
echo - imprime en pantalla un mensaje dado (echo [mensaje])\n \
    ");
}

static void fractals() {
  drawJuliaFractal(1024, 768, 0.83035, 0.087267, -0.013638, 128, -0.77098, -0.08545);
  //sleep(1000);
  //clear();
}

static void getTime() {
  int h = hour(), m = minutes(), s = seconds();
  printf("Hora actual - ");
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

static void echo(char * str) {
  printf("%s\n", str);
}
