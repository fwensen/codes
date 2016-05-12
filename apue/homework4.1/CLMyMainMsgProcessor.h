#ifndef CLMyMainMsgProcessor_H
#define CLMyMainMsgProcessor_H

#include "Common.h"
#include "CLThreadPool.h"
#include "CLMessageObserver.h"
#include "CLExecutiveNameServer.h"
#include "CLMessageLoopManager.h"

class CLMyMainMsgProcessor : public CLMessageObserver {

public:

    CLMyMainMsgProcessor(CLThreadPool *threadPool);
    virtual ~CLMyMainMsgProcessor();

    virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext);

    CLStatus On_AddMsg(CLMessage *pM);
    CLStatus On_QuitMsg(CLMessage *pM);

private:

    CLThreadPool* m_threadPool;
};

#endif
