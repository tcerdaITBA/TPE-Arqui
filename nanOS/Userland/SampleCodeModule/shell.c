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

#define MAX_CMD_SIZE 256 // 2^16 bytes buffer size.
#define CMD_SAVED 16
/*
Podemos hacer el buffer circular para que no tener que mover todo el buffer
cuando se le acaba la capacidad
*/

int shell() {
    int run = 1;
    char buffer[MAX_CMD_SIZE];
    int len = MAX_CMD_SIZE;
    //drawJuliaFractal(1024, 768, 0.83035, 0.0659505, -0.1220261, 128, -0.6548832, -0.4477065);


    while (run) {
        putchar('>');
        readline(buffer, len);
        execute(buffer);
    }
    printf("outwhile\n");
    return 0;
}

/*Array de punteros a funcion coincidente con la posicion de los nombres de los comandos*/
//static void (*functionsArray[10])() = {help,fractals,getTime, sleep};
/*Array de nombres de los comandos */
//static char* functionsName[10] = {"help", "fractals", "time", "clear"};

void execute(char * cmd) {
    int i = 0;
    //TODO: caso echo

    //if (strcmp(cmd, "echo") == 0) {    //caso de echo (i=0)
    //    printf("%s\n", cmd+5);
    //} else {

    if (strcmp(cmd, "fractals") == 0) {
      fractals();
    } else if (strcmp(cmd, "help") == 0) {
      help();
      //putchar('M');
    } else if (strcmp(cmd, "clear") == 0) {
      clear();
    } else if (strcmp(cmd, "time") == 0) {
      getTime();
    } else {
      printf("Command Not Found\n");
    }
/*
    for(i = 0; i < 10; i++) {
        if(strcmp(cmd,functionsName[i])){
             (*functionsArray[i])();
        }
    }
*/

}

static void help(){

  printf("HELP FUNCTION -- shows the principal User Commands and its description\n\n");
  printf(" 1- echo [args...]");
  printf("    Write arguments to the standard output. Display the args, separated by a single space character\n");
  printf("                      and followed by a newline, on the standard output.\n");

  printf(" 2- clear             Clear the terminal screen.\n");
  printf(" 3- time              Display the current time on the standard output using 24hr format [hh:mm:ss]\n");
  printf(" 4- fractals          Display a new fractal drawing on the standard output\n\n");

}

static void fractals() {
  drawJuliaFractal(1024, 768, 0.83035, 0.0659505, -0.1220261, 128, -0.6548832, -0.4477065);
}

static void getTime() {
  int h = hour(), m = minutes(), s = seconds();
  printf("Current Tme: %d:%d:%d\n", h, m, s);
}

static void clear() {
  int rows = 768/16;
  for (int i = 0; i < 2*rows; i++)
    printf("\n");

  putchar('\t');
}

static void echo(char * str) {
  printf("%s\n", str);
}
