#ifndef _CLQUITMESSAGE_H
#define _CLQUITMESSAGE_H
#include"CLMessage.h"

#define QUIT_MSG 1

class CLQuitMessage : public CLMessage {

public:

    CLQuitMessage() : CLMessage(QUIT_MSG)
	{
	}

	virtual ~CLQuitMessage()
	{
	}
};

#endif
