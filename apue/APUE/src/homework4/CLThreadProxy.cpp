#include<iostream>
#include"CLThreadProxy.h"
using namespace std;

CLThreadProxy::CLThreadProxy(CLExecutiveFunctionProvider *provider, bool bWaitForDeath) : CLThread(provider, bWaitForDeath) {

}
