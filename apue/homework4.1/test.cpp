#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLMessage.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLMessageObserver.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLExecutiveNameServer.h"
#include "CLExecutiveCommunication.h"
#include "CLMyCalcMsgProcessor.h"
#include "CLMyMainMsgProcessor.h"
#include "CLMyUserMsgProcessor.h"
#include "CLThreadPool.h"

using namespace std;

int main() {
	
	CLThreadPool *threadPool = new CLThreadPool(1);
	
	threadPool->initThreadPool();

    sleep(2);

	CLThread* userThread = new CLThread(
			new CLExecutiveFunctionForMsgLoop(
			new CLMsgLoopManagerForSTLqueue(
			new CLMyUserMsgProcessor(1000), "user")), true);
			
	userThread->Run(0);

	userThread->WaitForDeath();

	return 0;
}
