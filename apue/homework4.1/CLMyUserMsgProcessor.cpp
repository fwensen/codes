#include <iostream>
#include "CLMyUserMsgProcessor.h"

using namespace std;

CLMyUserMsgProcessor::CLMyUserMsgProcessor(int MsgNumber) {
		m_receiveMsg = 0;
		m_MsgNumber = MsgNumber;
}

CLMyUserMsgProcessor::~CLMyUserMsgProcessor() {}

CLStatus CLMyUserMsgProcessor::Initialize(CLMessageLoopManager *pMessageLoop, void* pContext) {
		
	pMessageLoop->Register(RESULT_MSG, (CallBackForMessageLoop)(&CLMyUserMsgProcessor::On_ResultMsg));
	pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLMyUserMsgProcessor::On_QuitMsg));

	for (int i=0; i<m_MsgNumber; i++) {
		CLExecutiveNameServer::PostExecutiveMessage("main", new CLAddMessage(i, i+1));
	}

	return CLStatus(0, 0);
}

CLStatus CLMyUserMsgProcessor::On_ResultMsg(CLMessage *pM) {
	
    CLResultMessage *pResultMsg = (CLResultMessage*)pM;
	
    cout << pResultMsg->m_Op1  << " + " << pResultMsg->m_Op2 << " = " << pResultMsg->m_Result << endl;
	
	m_receiveMsg++;
	
	if (m_receiveMsg == m_MsgNumber) {
		CLExecutiveNameServer::PostExecutiveMessage("main", new CLQuitMessage());
		return CLStatus(QUIT_MESSAGE_LOOP, 0);
	}
	
	return CLStatus(0, 0);
}

CLStatus CLMyUserMsgProcessor::On_QuitMsg(CLMessage* pM) {
	return CLStatus(QUIT_MESSAGE_LOOP, 0);
}
