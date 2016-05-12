#ifndef _CLADDMESSAGE_H
#define _CLADDMESSAGE_H
#include"CLMessage.h"
#include "CLMessage.h"
#include "CLExecutiveNameServer.h"
#include "CLQuitMessage.h"

#define ADD_MSG 0
#define QUIT_MSG 1
#define RESULT_MSG 2

using namespace std;

class CLAddMessage : public CLMessage {

public:

    CLAddMessage(int Op1, int Op2) : CLMessage(ADD_MSG) {
        m_Op1 = Op1;
        m_Op2 = Op2;
    }

    virtual ~CLAddMessage() {
/*
        if (!(CLExecutiveNameServer::PostExecutiveMessage("quit_message_loop", new CLQuitMessage())).IsSuccess())
            cout << "~CLAddMessage error" << endl;
*/  
	  }

    int m_Op1;
    int m_Op2;
private:

};

#endif
