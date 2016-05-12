#include<iostream>
#include "CLMyThreadPool.h"

using namespace std;

class CLMyCalculate;

CLMyThreadPool::CLMyThreadPool () {

    ThreadSum = 5;
    MainThreadName = "mainThread";
    ThreadHead = NULL;
}

CLMyThreadPool::CLMyThreadPool (int number) {

    if (number <= 0)
        ThreadSum = 5;
    else
        ThreadSum = number;

    MainThreadName = "mainThread";
    ThreadHead = NULL;
}

CLMyThreadPool::~CLMyThreadPool() {
    
}

void CLMyThreadPool::InitThreadPool() {
    
    CLThread *mainThread = new CLThread(
						   new CLExecutiveFunctionForMsgLoop(
                           new CLMsgLoopManagerForSTLqueue( 
                           new CLMain(this), MainThreadName.c_str())),  false);
    mainThread->Run(0);

    for (int i = 0; i < ThreadSum; ++i) {
        
        CLThreadLink *thread = new CLThreadLink;
        char t[32];
        string s;
        sprintf(t, "%d", i);
        s = t;
        thread->ThreadName = "thread_" + s;
        thread->Next = ThreadHead;
        ThreadHead = thread;

        CLThread *proxy = new CLThread(
						 new CLExecutiveFunctionForMsgLoop(
                         new CLMsgLoopManagerForSTLqueue (
                         new CLMyCalculate, (thread->ThreadName).c_str())),  false);
        proxy->Run(0);
    }

    CurrentThread = ThreadHead;
}

void CLMyThreadPool::PostMessageToMainThread(CLMessage *msg) {
    
    CLExecutiveNameServer::PostExecutiveMessage(MainThreadName.c_str(), msg);
}

