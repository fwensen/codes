#ifndef _THREAD_H
#define _THREAD_H
#include<pthread.h>
#include"client.h"
const int MAXLINE = 1024;

class ChatThread : public ChatClient {


public:

    ChatThread(char *, int);
    void run();
    ~ChatThread(){ close(sock); }
private:

    static int sock;
    static void *readWork(void*);
    ssize_t rio_writen(int fd, void *userbuf, size_t n);
};
#endif
