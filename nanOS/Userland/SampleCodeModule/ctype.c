#include "ctype.h"

int isalnum(int c) {
  return isalpha(c) || isdigit(c);
}

int isalpha(int c) {
  return islower(c) || isupper(c);
}

int isblank(int c) {
  return c == '\t' || c == ' ';
}

int iscntrl(int c) {
  return !isprint(c);
}

int isdigit(int c) {
  return c >= '0' && c <= '9';
}

int isgraph(int c) {
  return isprint(c) && c != ' ';
}

int islower(int c) {
  return c >= 'a' && c <= 'z';
}

int isprint(int c) {
  return c > 0x1F && c != 0x7F;
}

int ispunct(int c) {
  return isgraph(c) && !isalnum(c);
}

int isspace(int c) {
  return isblank(c) || c == '\v' || c == '\f' || c == '\r' || c == '\n';
}

int isupper(int c) {
  return c >= 'A' && c <= 'Z';
}

int isxdigit(int c) {
  return isdigit(c) || (c >= 'a' && c <= 'f') ||
          (c >= 'A' && c <= 'F');
}
