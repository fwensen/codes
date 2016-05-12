#ifndef _CLUSERTHREADPRO_H
#define _CLUSERTHREADPRO_H
#include "CLMessageObserver.h"
#include "CLStatus.h"
#include "CLMessageObserver.h"
#include "CLMessageLoopManager.h"
#include "CLExecutiveNameServer.h"
#include "CLMyThreadPool.h"

#define RESULT_MSG 2

class CLUser : public CLMessageObserver {

public:

    CLUser(CLMyThreadPool*);
    ~CLUser();
    CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void *);
    CLStatus Do_ResultMsg(CLMessage *);
    CLStatus Do_QuitMsg(CLMessage *);

private:


    CLMyThreadPool *Pool;
    int SendCount;
    int ReceivedResultCount;

};
#endif
