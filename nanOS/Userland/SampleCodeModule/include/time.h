#ifndef TIME_H
#define TIME_H

#define MIN_GMT -12
#define MAX_GMT 14
#define DEFAULT_GMT -3 // Argentina GMT

int seconds();
int minutes();
int hour();
int setGMT(int gmt);

#endif
