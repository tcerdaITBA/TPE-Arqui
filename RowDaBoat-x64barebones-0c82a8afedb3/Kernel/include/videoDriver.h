#ifndef __VIDEO_DRIVER__H__
#define __VIDEO_DRIVER__H__

void set_color(char color);
void print_str(const char *str, int row, int col);
void put_char(char c);
void put(const char *str, int len);
void print_char(char c, int row, int col);
void print_num(int num, int row, int col);
void print(const char *str, int len, int row, int col);
void clear();

#define DEFAULT_COLOR 0x0F
#define ERROR_COLOR 0X0C
#define CLEAR_COLOR 0x00

#define HEIGHT 25
#define WIDTH 80

#endif
