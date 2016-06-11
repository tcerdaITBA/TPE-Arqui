#include <stdio.h>
#include "strings.h"

int shell();
void execute(char * cmd);
char * get_option(char * cmd);

#define MAX_CMD_SIZE 256 // 2^16 bytes buffer size.
#define CMD_SAVED 16
/*
Podemos hacer el buffer circular para que no tener que mover todo el buffer
cuando se le acaba la capacidad
*/

int shell() {
    int run = 1;
    char c;
    char cmd_index = 0;
    char current_cmd[MAX_CMD_SIZE];
    char cmd_buffer[CMD_SAVED][MAX_CMD_SIZE];

    while (run) {
        c = getchar();
        if (c == '\n') {
            // ejecuta el comando ingresado
            current_cmd[cmd_index] = '\0';
            execute(current_cmd);
            cmd_index = 0;
        } else if (cmd_index < MAX_CMD_SIZE) {
            // imprime el caracter ingresado y lo guarda
            putchar(c);
            current_cmd[cmd_index++] = c;
        } else {
            // hacer algo cuando el comando queda muy grande.
        }
    }
}

void execute(char * cmd) {
    char * str1 = "echo";
    char option[16];
    int index = strcpynto(option, cmd, ' ', 15);
    option[index] = '\0';
    if (strcmp(option, "echo") == 0) {
        printf("%s\n", cmd+index);
    }
}

char * get_option(char * cmd) {

}

int main(int argc, char const *argv[]) {
    shell();
    return 0;
}
