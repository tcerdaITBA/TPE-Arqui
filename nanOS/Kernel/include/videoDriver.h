#ifndef VIDEO_DRIVER__H
#define VIDEO_DRIVER__H

#include "font.h"

void set_color(char color);
void print_str(const char *str, int row, int col);
char current_color();
void put_char(char c);
void put(const char *str, int len);
void put_str(const char *str);
void print_char(char c, int row, int col);
void print_num(int num, int row, int col);
void print(const char *str, int len, int row, int col);
int fill(char red, char green, char blue, int x, int y);
void clear();
int scr_x_res();
int scr_y_res();
int text_rows();
int text_cols();

// TODO NO SE COMO HACER PARA QUE SE DEFINA ESTO UNA VEZ SOLA LLAMANDO A LA FUNCION
#define SCREEN_HEIGHT scr_y_res()
#define SCREEN_WIDTH  scr_x_res()

#define HEIGHT (SCREEN_HEIGHT/CHAR_HEIGHT)
#define WIDTH (SCREEN_WIDTH/CHAR_WIDTH)

#define DEFAULT_COLOR 0x0F
#define ERROR_COLOR 0X0C
#define CLEAR_COLOR 0x00


#endif
