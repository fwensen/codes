#ifndef _CLIENT_H
#define _CLIENT_H
#include"socketWrap.h"
class ChatClient {

public:

    ChatClient(char *, int);
    ~ChatClient(){}

protected:

    int initClient();
    static int clientFd;
    static char* groupName; 
    static char *userName;

private:
    char ip[32];
    int port;

};

#endif
