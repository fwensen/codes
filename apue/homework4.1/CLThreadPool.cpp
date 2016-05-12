#include <string>
#include "CLThread.h"
#include "CLThreadPool.h"
#include "CLMyMainMsgProcessor.h"
#include "CLMyCalcMsgProcessor.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLExecutiveFunctionForMsgLoop.h"
#include "CLExecutiveNameServer.h"

using namespace std;

CLThreadPool::CLThreadPool(int threadNum) {
    m_threadNum = threadNum;
}

CLThreadPool::~CLThreadPool() {}

void CLThreadPool::initThreadPool() {
    
    // for mainThread
    m_mainThread = new CLThread(
                   new CLExecutiveFunctionForMsgLoop(
                   new CLMsgLoopManagerForSTLqueue(
                   new CLMyMainMsgProcessor(this), "main")), true);
    
    m_mainThread->Run(0);

    // for calcThread
    for (int i=0; i<m_threadNum; i++) {
    
        char id[10];
        sprintf(id, "%d", i);
        string name(id);
        name = "calc" + name;
        
        CLThread* calcThread = new CLThread(
                new CLExecutiveFunctionForMsgLoop(
                new CLMsgLoopManagerForSTLqueue(
                new CLMyCalcMsgProcessor, name.c_str())), true);
                
        calcThreadQueue.push(calcThread);
        
        calcThread->Run(0);
    }
}

