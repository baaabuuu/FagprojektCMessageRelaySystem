#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "charQueue.h"

void* startLoop1(void *arg_queue);
void* startLoop2(void *arg_queue);
int main();


#endif // _MAIN_H_
