#ifndef _CLMYTHREADPOLL_H
#define _CLMYTHREADPOLL_H
#include<stdio.h>
#include<string>
#include"CLThreadLink.h"
#include "CLMyCalculatePro.h"
#include "CLResultMessage.h"
#include "CLThreadProxy.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLMessage.h"
#include "CLMyMainPro.h"

class CLMyThreadPool{

public:

    CLMyThreadPool();
    CLMyThreadPool(int);
    ~CLMyThreadPool();

    void InitThreadPool();
    void UninitThreadPool();
    void PostMessageToMainThread(CLMessage *msg);

    int ThreadSum;
    std::string MainThreadName;
    CLThreadLink *ThreadHead;

    CLThreadLink *CurrentThread;
private:

};
#endif
