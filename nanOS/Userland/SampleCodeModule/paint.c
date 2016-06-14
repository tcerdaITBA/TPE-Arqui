#include "syscalls.h"
#include "stdio.h"

int paint_pixel(char red, char green, char blue, int x, int y) {
  int color = 0;
  color += red;
  color = color << 8;
  color += green;
  color = color << 8;
  color += blue;
  paint(color, x, y);
}
