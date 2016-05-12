#include<iostream>
#include"CLUserThreadPro.h"
using namespace std;

CLUser::CLUser(CLMyThreadPool *pool) {

    this->Pool = pool;
}

CLUser::~CLUser() {}

CLStatus CLUser::Initialize(CLMessageLoopManager *pMessageLoop, void *pContext) {
    pMessageLoop->Register(RESULT_MSG, (CallBackForMessageLoop)(&CLUser::Do_ResultMsg));
    SendCount = 1000;
    pMessageLoop->Register(QUIT_MSG, (CallBackForMessageLoop)(&CLUser::Do_QuitMsg));
    ReceivedResultCount = 0;
    for (int i = 0; i < SendCount; ++i) {
        
        Pool->PostMessageToMainThread(new CLAddMessage( i, i));
    }

    return CLStatus(0, 0);
}

CLStatus CLUser::Do_ResultMsg(CLMessage *pM) {
    CLResultMessage *pResult = (CLResultMessage*)pM;
    cout << pResult->m_Op1 << "+" << pResult->m_Op2 << "=" << pResult->m_Result << endl;
    ReceivedResultCount++;
    if (ReceivedResultCount == SendCount) {
        Pool->PostMessageToMainThread(new CLQuitMessage());
        return CLStatus(QUIT_MESSAGE_LOOP, 0);
    }
    return CLStatus(0, 0);
}

CLStatus CLUser::Do_QuitMsg(CLMessage *pM) {
    return CLStatus(QUIT_MESSAGE_LOOP, 0);
}
