#include<iostream>
#include "./CLMyMainPro.h"
using namespace std;

CLMain::CLMain(CLMyThreadPool *tPool) {
    this->threadPool = tPool;
}

CLMain::~CLMain() {

}

CLStatus CLMain::Initialize(CLMessageLoopManager *pMessageLoop, void *pContext) {

    pMessageLoop->Register(ADD_MSG, (CallBackForMessageLoop)(&CLMain::Do_PostMsgToThreadPool));
    pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLMain::Do_QuitMsg));
    return CLStatus(0, 0);
}

CLStatus CLMain::Do_PostMsgToThreadPool(CLMessage *pM) {
    CLAddMessage *p = (CLAddMessage*)pM;
    CLMessage *pp = new CLAddMessage(p->m_Op1, p->m_Op2);
    CLExecutiveNameServer::PostExecutiveMessage(threadPool->CurrentThread->ThreadName.c_str(), pp);

    threadPool->CurrentThread = threadPool->CurrentThread->Next;
    if (threadPool->CurrentThread == NULL)
        threadPool->CurrentThread = threadPool->ThreadHead;
    return CLStatus(0, 0);
}

CLStatus CLMain::Do_QuitMsg(CLMessage *pM) {
    CLThreadLink *p = threadPool->ThreadHead;
    while(p) {
        CLQuitMessage *q = new CLQuitMessage();
        CLExecutiveNameServer::PostExecutiveMessage(p->ThreadName.c_str(), q);
        p = p->Next;
    }
    return CLStatus(QUIT_MESSAGE_LOOP, 0);
}

