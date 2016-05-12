#ifndef CLMyUserMsgProcessor_H
#define CLMyUserMsgProcessor_H

#include "Common.h"
#include "CLStatus.h"
#include "CLThreadPool.h"
#include "CLMessageObserver.h"
#include "CLMessageLoopManager.h"
#include "CLExecutiveNameServer.h"

class CLMyUserMsgProcessor : public CLMessageObserver {

public:

	CLMyUserMsgProcessor(int MsgNumber);
	virtual ~CLMyUserMsgProcessor();

	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext);

	CLStatus On_ResultMsg(CLMessage *pM);
	CLStatus On_QuitMsg(CLMessage* pM);

private:
    int m_receiveMsg;
    int m_MsgNumber;
};

#endif
