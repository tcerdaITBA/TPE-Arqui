#include "stdio.h"
#include "strings.h"
#include "fractals.h"
#include "time.h"
#include "shell.h"

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
      printf("in fractals\n");
      fractals();
      paint_pixel(255, 255, 255, 50, 50);
      //putchar('M');
      //printf("out fractals\n");
    } else if (strcmp(cmd, "help") == 0) {
      help();
      //putchar('M');
    } else if (strcmp(cmd, "clear") == 0) {
      clear();
    } else {
      printf("unsupported\n");
    }
/*
    for(i = 0; i < 10; i++) {
        if(strcmp(cmd,functionsName[i])){
             (*functionsArray[i])();
        }
    }
*/

}

//Funciones vacias para probar que compile
static void help(){
  int i = 100;
  int count = 0;
  while(count < 1) {
    for (int k = 0; k < 1024; k++) {
      for (int l = 0; l < 768; l++) {
        i *= 33;
        paint_pixel(i%200, i%255, (i+4980)%200, k, l);
      }
    }
    count++;
  }
  //printf("FUNCIONES DISPONIBLES:\n1- fractals\n2-clock\n"); // TODO
}

static void fractals() {
  drawJuliaFractal(1024, 768, 0.83035, 0.0659505, -0.1220261, 128, -0.6548832, -0.4477065);
}

static void getTime() {
  int h = hour(), m = minutes(), s = seconds();
  printf("Hora actual: %d:%d:%d\n", h, m, s);
}

static void clear() {
  int rows = 768/16;
  for (int i = 0; i < rows; i++)
    printf("\n");

  putchar('\t');
}

static void echo(char * str) {
  printf("%s\n", str);
}
