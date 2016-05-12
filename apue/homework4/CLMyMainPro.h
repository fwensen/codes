#ifndef _CLMYMAINPRO_H
#define _CLMYMAINPRO_H
#include "CLThread.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLMessageObserver.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLExecutiveNameServer.h"
#include "CLExecutiveCommunication.h"
#include "CLMyThreadPool.h"

#define ADD_MSG 0
#define QUIT_MSG 1
#define RESULT_MSG 2

class CLMyThreadPool;

class CLMain : public CLMessageObserver {

public:
    CLMain(CLMyThreadPool*);
    ~CLMain();

    CLStatus Initialize(CLMessageLoopManager *, void *);
    CLStatus Do_PostMsgToThreadPool(CLMessage *);
    CLStatus Do_QuitMsg(CLMessage *);

    CLMyThreadPool* threadPool;
private:

};
#endif
