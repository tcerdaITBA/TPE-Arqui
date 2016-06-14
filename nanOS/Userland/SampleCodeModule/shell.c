#include "stdio.h"
#include "strings.h"

int shell();
void execute(char * cmd);
void help();
void fractals();
void time();

#define MAX_CMD_SIZE 256 // 2^16 bytes buffer size.
#define CMD_SAVED 16
/*
Podemos hacer el buffer circular para que no tener que mover todo el buffer
cuando se le acaba la capacidad
*/

int shell() {
    int run = 1;
    char buffer[MAX_CMD_SIZE]:
    int len = MAX_CMD_SIZE;

    while (run) {
        readline(buffer, len);
        
        execute(buffer);
    }
    return 0;
}

/*Array de punteros a funcion coincidente con la posicion de los nombres de los comandos*/
static void (*functionsArray[10])() = {help,fractals,getTime, sleep};
/*Array de nombres de los comandos */
static char* functionsName[10] = {"help", "fractals", "time", "sleep"};

void execute(char * cmd) {
    int i = 0;
    //TODO: caso echo 
    
    //if (strcmp(cmd, "echo") == 0) {    //caso de echo (i=0)
    //    printf("%s\n", cmd+5);
    //} else {
 
    for(i = 0; i < 10; i++) {
        if(strcmp(cmd,functionsName[i])){
             (*functionsArray[i])(); 
        }
    }


}

//Funciones vacias para probar que compile
void help(){

}

void fractals(){

}

void getTime() {

}

void sleep() {

}
//int main(int argc, char const *argv[]) {
//  shell();
//}
