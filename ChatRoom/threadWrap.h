#ifndef _THREADWRAP_H
#define _THREADWRAP_H
#include<pthread.h>
#include<string.h>
#include<errno.h>

void CreateThread(pthread_t *pid, void* func, void *arg);
void JoinThread(pthread_t tid, void**);
void ExitThread(void *);

#endif
