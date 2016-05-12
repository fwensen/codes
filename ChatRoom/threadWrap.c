#include<stdio.h>
#include"threadWrap.h"

void CreateThread(pthread_t *pid, void *func, void *arg){

    int ret = pthread_create(pid, NULL, func, arg);
    if (ret != 0) {
        sprintf("Thread create error: %s\n", strerror(errno));
        return;
    }
}


void JoinThread(pthread_t tid, void **status) {
    
    int ret = pthread_join(tid, status);
    if (ret != 0) {
        sprintf("Thread join error: %s\n", strerror(errno));
        return;
    }
}

void ExitThread(void *status) {
    
    pthread_exit(status);
}
