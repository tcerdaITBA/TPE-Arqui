#include "stdio.h"
#include "strings.h"
#include "fractals.h"
#include "shell.h"
#include "commands.h"
#include "stdlib.h"
#include "syscalls.h"
#include "process_info.h"
#include "executer.h"

#define UNSUPPORTED_COMMAND "Unsupported Command\n"
#define INVALID_ARGUMENTS "Invalid Arguments\n"

#define MAX_CMD_SIZE 256

static int extract_cmd_name(char * cmd_name, const char * str);
static void handle_validity(int valid);

int shell() {
    int run = 1;
    char buffer[MAX_CMD_SIZE];
    char cmd_name[MAX_CMD_SIZE];
    int len = MAX_CMD_SIZE;
    int name_len;
    int valid;
    int arguments_flag; // 1 si se enviaron argumentos

    load_fractals(data_address());
    set_fractals_resolution(screen_Xresolution(), screen_Yresolution()); // fractales en resolucion bien manija

    execpn(process_info_manager); /* Proceso que corre en background guardando los pid que se van creando y destruyendo */

    while (run) {
        printf("> ");
        readline_no_spaces(buffer, len);
        if (buffer[0] != '\0') { // Se escribió algo
          name_len = extract_cmd_name(cmd_name, buffer);
          arguments_flag = buffer[name_len] != '\0';
          valid = execute(cmd_name, buffer+name_len+arguments_flag); // Nombre y argumentos
          handle_validity(valid);
        }
    }

    return 0;
}

/* Retorna la longitud del comando ingresado y guarda su contenido en un vector recibido como parámetro */
static int extract_cmd_name(char * cmd_name, const char * str) {
  int i;
  for (i = 0; str[i] != '\0' && str[i] != ' '; i++)
    cmd_name[i] = str[i];
  cmd_name[i] = '\0';
  return i;
}

/* Imprime en pantalla en caso de haber ingresado un comando inválido */
static void handle_validity(int valid) {
  switch (valid) {
    case UNSUPPORTED:
      printf("%s", UNSUPPORTED_COMMAND);
      break;
    case INVALID_ARGS:
      printf("%s", INVALID_ARGUMENTS);
      break;
  }
}