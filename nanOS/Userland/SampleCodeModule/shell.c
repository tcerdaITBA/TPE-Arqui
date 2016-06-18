#include "stdio.h"
#include "strings.h"
#include "fractals.h"
#include "time.h"
#include "shell.h"
#include "paint.h"
#include "commands.h"

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

    while (run) {
        printf("> ");
        readline_no_spaces(buffer, len);
        if (buffer[0] != '\0') { // Se escribio algo
          name_len = extract_cmd_name(cmd_name, buffer);
          valid = execute(cmd_name, buffer+name_len); // Nombre y argumentos
          handle_validity(valid);
        }
    }

    return 0;
}



static int extract_cmd_name(char * cmd_name, const char * str) {
  int i;
  for (i = 0; str[i] != '\0' && str[i] != ' '; i++)
    cmd_name[i] = str[i];
  cmd_name[i] = '\0';
  return i;
}

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
