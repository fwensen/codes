#include <iostream>
#include "CLThread.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLMessageObserver.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLExecutiveNameServer.h"
#include "CLExecutiveCommunication.h"
#include "CLMyMainPro.h"
#include "CLMyThreadPool.h"
#include "CLUserThreadPro.h"

using namespace std;

#define ADD_MSG 0
#define QUIT_MSG 1


int main()
{

    CLMyThreadPool *pool = new CLMyThreadPool(10);
    pool->InitThreadPool();

    sleep(2);

    CLThread* userThread = new CLThread(
			new CLExecutiveFunctionForMsgLoop(
			new CLMsgLoopManagerForSTLqueue(
			new CLUser(pool), "user_thread")), true);
			
	userThread->Run(0);
    userThread->WaitForDeath();
	return 0;
}
