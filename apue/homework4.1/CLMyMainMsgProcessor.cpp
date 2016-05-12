#include <iostream>
#include "CLMyMainMsgProcessor.h"
using namespace std;

CLMyMainMsgProcessor::CLMyMainMsgProcessor(CLThreadPool *threadPool) {
    m_threadPool = threadPool;
}
    
CLMyMainMsgProcessor::~CLMyMainMsgProcessor() {}

CLStatus CLMyMainMsgProcessor::Initialize(CLMessageLoopManager *pMessageLoop, void* pContext) { 
    
    pMessageLoop->Register(ADD_MSG, (CallBackForMessageLoop)(&CLMyMainMsgProcessor::On_AddMsg));
    pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLMyMainMsgProcessor::On_QuitMsg));
    
    return CLStatus(0, 0);
}

CLStatus CLMyMainMsgProcessor::On_AddMsg(CLMessage *pM) {

    CLAddMessage *pAddMsg = (CLAddMessage*)pM;
    CLAddMessage *ppAddMsg = new CLAddMessage(pAddMsg->m_Op1, pAddMsg->m_Op2);
    CLExecutiveNameServer::PostExecutiveMessage("calc0", ppAddMsg);
}

CLStatus CLMyMainMsgProcessor::On_QuitMsg(CLMessage *pM) {
        
    for (int i=0; i<m_threadPool->calcThreadQueue.size(); i++) {
        
        CLQuitMessage *pQuitMsg = new CLQuitMessage();
            
        char id[10];
        sprintf(id, "%d", i);
        string name(id);
        name = "calc" + name;
            
        CLExecutiveNameServer::PostExecutiveMessage(name.c_str(), pQuitMsg);
    }
        
    return CLStatus(QUIT_MESSAGE_LOOP, 0);
}

