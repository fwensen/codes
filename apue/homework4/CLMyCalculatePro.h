#ifndef _CLMYCALCULATEPRO_H
#define _CLMYCALCULATEPRO_H
#include "CLThread.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLMessageObserver.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLExecutiveNameServer.h"
#include "CLExecutiveCommunication.h"
#include "CLAddMessage.h"
#include "./CLResultMessage.h"

#define ADD_MSG 0
#define QUIT_MSG 1
#define RESULT_MSG 2

class CLMyCalculate : public CLMessageObserver {

public:

    CLMyCalculate();
    ~CLMyCalculate();
    CLStatus Initialize(CLMessageLoopManager *pMsgLoop, void *pContext);
    CLStatus Do_AddMsg(CLMessage *pM);
    CLStatus Do_QuitMsg(CLMessage *pM);

private:

};
#endif
