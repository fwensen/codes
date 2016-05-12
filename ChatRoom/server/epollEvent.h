#ifndef _CHATSERVER2_H
#define _CHATSERVER2_H
#include<sys/epoll.h>
#include<map>
#include<string>
#include"GroupChat.h"
const int MAXEVENTS = 1000;


class EpollEvent : public GroupChat{

public:

    EpollEvent(int);
    void doWork();
    ~EpollEvent(){};

private:

    void createEvent();
    void addEvent(int sock);
    int waitEvent();

private:

    static struct epoll_event event;
    static struct epoll_event events[MAXEVENTS];
    static int eventFd;
};
#endif
