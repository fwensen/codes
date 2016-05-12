#ifndef CLThreadPool_H
#define CLThreadPool_H

#include <queue>
#include <stdio.h>
#include "Common.h"
#include "CLThread.h"

using namespace std;

class CLThreadPool {

public:
    CLThreadPool(int threadNum);
    ~CLThreadPool();

    void initThreadPool();

public: 
    queue<CLThread*> calcThreadQueue;

private:
    int m_threadNum;
    CLThread* m_mainThread;
};

#endif
