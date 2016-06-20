/* Fuentes:
** HSV to RGB -> http://www.splinter.com.au/converting-hsv-to-rgb-colour-using-c/
** Fractal de Julia -> http://lodev.org/cgtutor/juliamandelbrot.html
*/
#include "paint.h"
#include "fractals.h"
#include "syscalls.h"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

static int screen_width = DEFAULT_WIDTH;
static int screen_height = DEFAULT_HEIGHT;

static void HsvToRgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v);

#define FRACTALS_SIZE (sizeof(param)/sizeof(param[0]))

struct fractalParams{
  double zoom;
  double moveX;
  double moveY;
  unsigned int maxIterations;
  double cRe;
  double cIm;
} param[] = {
          {1265.76110029, 0.427834925, 0.42325986, 128, -0.6548832, -0.4477065},
          {0.85035, 0.437267, -0.613638, 120, -0.47098, -0.68545},
          {0.83035, 0.087267, -0.013638, 128, -0.77098, -0.08545},
          {1265.7611, 0.20932, 0.10459, 256, -0.6448832, -0.447706},
          {0.830350, 0.06595, -0.122026, 128, -0.654883, -0.4477065},
          {1265.76110029, 0.2093925, 0.1045986, 128, -0.6548832, -0.4477065}
        };

int fractals_size() {
  return FRACTALS_SIZE;
}

int draw_fractal(int index) {
  if (index < 0 || index >= FRACTALS_SIZE)
    return 0;
  drawJuliaFractal(param[index].zoom, param[index].moveX, param[index].moveY, param[index].maxIterations, param[index].cRe, param[index].cIm);
  return 1;
}

int set_fractals_resolution(int w_res, int h_res) {
  if (w_res < 0 || h_res < 0 || w_res > screen_Xresolution() || h_res > screen_Yresolution())
    return 0; // resolucion invalida

  screen_width = w_res;
  screen_height = h_res;
  return 1;
}

void drawJuliaFractal(double zoom, double moveX, double moveY, unsigned int maxIterations, double cRe, double cIm)
{

  //each iteration, it calculates: new = old*old + c, where c is a constant and old starts at current pixel
  //real and imaginary part of the constant c, determinate shape of the Julia Set

  double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old
  //you can change these to zoom and change position
  //after how much iterations the function should stop

  //pick some values for the constant c, this determines the shape of the Julia Set

  //loop through every pixel
  for(int y = 0; y < screen_height; y++)
  for(int x = 0; x < screen_width; x++)
  {
    //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
    newRe = 1.5 * (x - screen_width / 2) / (0.5 * zoom * screen_width) + moveX;
    newIm = (y - screen_height / 2) / (0.5 * zoom * screen_height) + moveY;
    //i will represent the number of iterations
    int i;
    //start the iteration process
    for(i = 0; i < maxIterations; i++)
    {
      //remember value of previous iteration
      oldRe = newRe;
      oldIm = newIm;
      //the actual iteration, the real and imaginary part are calculated
      newRe = oldRe * oldRe - oldIm * oldIm + cRe;
      newIm = 2 * oldRe * oldIm + cIm;
      //if the point is outside the circle with radius 2: stop
      if((newRe * newRe + newIm * newIm) > 4) break;
    }
    //use color model conversion to get rainbow palette, make brightness black if maxIterations reached
    unsigned char outr, outg, outb;
    HsvToRgb(&outr, &outg, &outb, i % 256, 255, 255 * (i < maxIterations));

    //draw the pixel
    paint_pixel(outr, outg, outb, x, y);
  }
}

static void HsvToRgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v)
{
    unsigned char region, fpart, p, q, t;

    if(s == 0) {
        /* color is grayscale */
        *r = *g = *b = v;
        return;
    }

    /* make hue 0-5 */
    region = h / 43;
    /* find remainder part, make it from 0-255 */
    fpart = (h - (region * 43)) * 6;

    /* calculate temp vars, doing integer multiplication */
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * fpart) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;

    /* assign temp vars based on color cone region */
    switch(region) {
        case 0:
            *r = v; *g = t; *b = p; break;
        case 1:
            *r = q; *g = v; *b = p; break;
        case 2:
            *r = p; *g = v; *b = t; break;
        case 3:
            *r = p; *g = q; *b = v; break;
        case 4:
            *r = t; *g = p; *b = v; break;
        default:
            *r = v; *g = p; *b = q; break;
    }

    return;
}
