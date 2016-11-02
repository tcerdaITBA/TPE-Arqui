#ifndef _MUTEX_H_
#define _MUTEX_H_

#define MAX_MUTEXES 64

#define LOCKED 0;
#define UNLOCKED 1;

#define BLOCKED 0;
#define READY 1;

#define NOT_OPEN -1;

int mutex_open(char * name);
int mutex_lock(int key);
int mutex_unlock(int key);
int mutex_close(int key);

#endif
