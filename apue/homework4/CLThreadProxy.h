#ifndef _CLTHREADPROXY_H
#define _CLTHREADPROXY_H
#include<string>
#include "CLThread.h"
#include "./CLExecutiveFunctionProvider.h"

class CLThreadProxy : public CLThread {

public:

    CLThreadProxy(CLExecutiveFunctionProvider *provider,  bool bWaitForDeath);
    ~CLThreadProxy(){}

private:

};

#endif
