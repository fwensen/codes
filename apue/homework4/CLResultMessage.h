#ifndef _CLRESULTMESSAGE_H
#define _CLRESULTMESSAGE_H
#include "CLMessage.h"

//#define ADD_MSG 0
//#define QUIT_MSG 1
#define RESULT_MSG 2
class CLResultMessage : public CLMessage
{
public:
    friend class CLMyUserMsgProcessor;
    friend class CLMyCalcMsgProcessor;
	CLResultMessage(int Op1, int Op2, int Result):CLMessage(RESULT_MSG)
	{
		m_Op1 = Op1;
		m_Op2 = Op2;
		m_Result = Result;
	}

	virtual ~CLResultMessage() {}

	int m_Op1;
	int m_Op2;
	int m_Result;
};
#endif
