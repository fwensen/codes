#include <iostream>
#include "CLMyCalcMsgProcessor.h"
using namespace std;

CLMyCalcMsgProcessor::CLMyCalcMsgProcessor() {}
    
CLMyCalcMsgProcessor::~CLMyCalcMsgProcessor() {};

CLStatus CLMyCalcMsgProcessor::Initialize(CLMessageLoopManager *pMessageLoop, void* pContext) {

    pMessageLoop->Register(ADD_MSG, (CallBackForMessageLoop)(&CLMyCalcMsgProcessor::On_AddMsg));
    pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLMyCalcMsgProcessor::On_QuitMsg));

    return CLStatus(0, 0);
}

CLStatus CLMyCalcMsgProcessor::On_AddMsg(CLMessage *pM) {
    
    CLAddMessage *pAddMsg = (CLAddMessage*)pM;

    int result = pAddMsg->m_Op1 + pAddMsg->m_Op2;
        
    CLResultMessage *pResultMsg = new CLResultMessage(pAddMsg->m_Op1, pAddMsg->m_Op2, result);
        
    CLExecutiveNameServer::PostExecutiveMessage("user", pResultMsg);

    return CLStatus(0, 0);
}

CLStatus CLMyCalcMsgProcessor::On_QuitMsg(CLMessage *pM) {
    return CLStatus(QUIT_MESSAGE_LOOP, 0);
}
