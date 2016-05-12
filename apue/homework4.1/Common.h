#ifndef Common_H
#define Common_H
#include "CLMessage.h"
#define ADD_MSG 0
#define QUIT_MSG 1
#define RESULT_MSG 2

class CLAddMessage : public CLMessage
{
public:
    friend class CLMyMainMsgProcessor;
    friend class CLMyCalcMsgProcessor;
	CLAddMessage(int Op1, int Op2):CLMessage(ADD_MSG)
	{
		m_Op1 = Op1;
		m_Op2 = Op2;
	}

	virtual ~CLAddMessage()
	{
	}

private:
	int m_Op1;
	int m_Op2;
};

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

private:
	int m_Op1;
	int m_Op2;
	int m_Result;
};

class CLQuitMessage : public CLMessage
{
public:
	CLQuitMessage() : CLMessage(QUIT_MSG)
	{
	}

	virtual ~CLQuitMessage()
	{
	}
};

#endif
