/* Fuentes:
** HSV to RGB -> http://www.splinter.com.au/converting-hsv-to-rgb-colour-using-c/
** Fractal de Julia -> http://lodev.org/cgtutor/juliamandelbrot.html
*/
#include "paint.h"
#include "fractals.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

static int Clamp(int i);
static void HsvToRgb(double h, double S, double V, int *r, int *g, int *b);

void drawJuliaFractal()
{
  //each iteration, it calculates: new = old*old + c, where c is a constant and old starts at current pixel
  double cRe, cIm;           //real and imaginary part of the constant c, determinate shape of the Julia Set
  double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old
  double zoom = 1, moveX = 0.087, moveY = -0.014; //you can change these to zoom and change position
  int maxIterations = 10; //after how much iterations the function should stop

  //pick some values for the constant c, this determines the shape of the Julia Set
  cRe = -0.77097;
  cIm = -0.08545;

  //loop through every pixel
  for(int y = 0; y < SCREEN_HEIGHT; y++)
  for(int x = 0; x < SCREEN_WIDTH; x++)
  {
    //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
    newRe = 1.5 * (x - SCREEN_WIDTH / 2) / (0.5 * zoom * SCREEN_WIDTH) + moveX;
    newIm = (y - SCREEN_HEIGHT / 2) / (0.5 * zoom * SCREEN_HEIGHT) + moveY;
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
    int * outr, * outg, * outb;
    HsvToRgb(i % 256, 255, 255 * (i < maxIterations), outr, outg, outb);
    //draw the pixel
    paint_pixel(*outr, *outg, *outb, x, y);
  }
}

static void HsvToRgb(double h, double S, double V, int *r, int *g, int *b)
{
  double H = h;
  while (H < 0) { H += 360; };
  while (H >= 0) { H -= 360; };

  double R, G, B;
  if (V <= 0) {
    R = G = B = 0;
  }
  else if (S <= 0)
  {
    R = G = B = V;
  }
  else
  {
    double hf = H / 60.0;
    int i = (int)hf;
    double f = hf - i;
    double pv = V * (1 - S);
    double qv = V * (1 - S * f);
    double tv = V * (1 - S * (1 - f));
    switch (i)
    {

      // Red is the dominant color

      case 0:
        R = V;
        G = tv;
        B = pv;
        break;

      // Green is the dominant color

      case 1:
        R = qv;
        G = V;
        B = pv;
        break;
      case 2:
        R = pv;
        G = V;
        B = tv;
        break;

      // Blue is the dominant color

      case 3:
        R = pv;
        G = qv;
        B = V;
        break;
      case 4:
        R = tv;
        G = pv;
        B = V;
        break;

      // Red is the dominant color

      case 5:
        R = V;
        G = pv;
        B = qv;
        break;

      // Just in case we overshoot on our math by a little, we put these here. Since its a switch it won't slow us down at all to put these here.

      case 6:
        R = V;
        G = tv;
        B = pv;
        break;
      case -1:
        R = V;
        G = pv;
        B = qv;
        break;

      // The color is not defined, we should throw an error.

      default:
        //LFATAL("i Value error in Pixel conversion, Value is %d", i);
        R = G = B = V; // Just pretend its black/white
        break;
    }
  }
  *r = Clamp((int)(R * 255.0));
  *g = Clamp((int)(G * 255.0));
  *b = Clamp((int)(B * 255.0));
}

static int Clamp(int i)
{
  if (i < 0) return 0;
  if (i > 255) return 255;
  return i;
}
