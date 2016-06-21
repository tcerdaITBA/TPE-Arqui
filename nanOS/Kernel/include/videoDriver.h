#ifndef VIDEO_DRIVER__H
#define VIDEO_DRIVER__H

#include "font.h"

void load_vDriver();

void print_str(const char *str, int row, int col);
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

#define SCREEN_HEIGHT y_res
#define SCREEN_WIDTH  x_res

#define HEIGHT (SCREEN_HEIGHT/CHAR_HEIGHT)
#define WIDTH (SCREEN_WIDTH/CHAR_WIDTH)

#endif
