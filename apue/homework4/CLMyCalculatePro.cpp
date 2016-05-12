#include<iostream>
#include"./CLMyCalculatePro.h"
using namespace std;

CLMyCalculate::CLMyCalculate() {

}

CLMyCalculate::~CLMyCalculate(){

}

CLStatus CLMyCalculate::Initialize(CLMessageLoopManager *pMsgLoop, void *pContext) {
    
    pMsgLoop->Register(ADD_MSG, (CallBackForMessageLoop)(&CLMyCalculate::Do_AddMsg));
    pMsgLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLMyCalculate::Do_QuitMsg));
    return CLStatus(0, 0);
}

CLStatus CLMyCalculate::Do_AddMsg(CLMessage *pM) {
    
    CLAddMessage *pAddMsg = (CLAddMessage*)pM;
    int result = pAddMsg->m_Op1 + pAddMsg->m_Op2;
    CLResultMessage *pResultMsg = new CLResultMessage(pAddMsg->m_Op1, pAddMsg->m_Op2, result);
    CLExecutiveNameServer::PostExecutiveMessage("user_thread", pResultMsg);
    return CLStatus(0, 0);
}

CLStatus CLMyCalculate::Do_QuitMsg(CLMessage *pM) {
    
    return CLStatus(QUIT_MESSAGE_LOOP, 0);
}
