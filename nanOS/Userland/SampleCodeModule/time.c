#include "syscalls.h"
#include "stdio.h"

int GMT = -3;

int seconds() {
    return time(0);
}

int minutes() {
    return time(1);
}

int hour() {
    return time(2) + GMT;
}
