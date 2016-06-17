#ifndef __STRINGS__H__
#define __STRINGS__H__

int str_len(const char *str);
int strcmp(const char * s, const char * t);
int strncmp(const char * s, const char * t, unsigned int n);
int strcpyto(char * to, const char * from, char limit);
int strcpynto(char * to, const char * from, char limit, unsigned int n);
int itoa(int value, char *str, int base);
int atoi(const char *str);

#endif
