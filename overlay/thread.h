#ifndef _THREAD_H
#define _THREAD_H
#include<pthread.h>
typedef void* (*THREAD_FUNC) (void*);

class Thread {

public:

    Thread(THREAD_FUNC, void*);
    ~Thread();

private:

    static pthread_t tid;
    Thread(const Thread&);
    //Thread& operator=(const Thread&);

};

#endif
