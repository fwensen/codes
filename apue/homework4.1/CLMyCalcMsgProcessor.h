#ifndef CLMyCalcMsgProcessor_H
#define CLMyCalaMsgProcessor_H

#include "Common.h"
#include "CLStatus.h"
#include "CLMessageObserver.h"
#include "CLMessageLoopManager.h"
#include "CLExecutiveNameServer.h"

class CLMyCalcMsgProcessor : public CLMessageObserver {

public: 
    
    CLMyCalcMsgProcessor();
    virtual ~CLMyCalcMsgProcessor();

    virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext);

    CLStatus On_AddMsg(CLMessage *pM);
    CLStatus On_QuitMsg(CLMessage *pM);
};

#endif
