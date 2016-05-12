#include<iostream>
#include<cstdlib>
#include "thread.h"

using namespace std;

pthread_t Thread::tid;

Thread::Thread(THREAD_FUNC func, void *arg) {

    int ret;
    ret = pthread_create(&tid, NULL, func, arg);
    if (ret != 0) {
        cout << "pthread_create error!" << endl;
        exit(-1);
    }
}

Thread::~Thread() {
    
    pthread_exit(NULL);
}
