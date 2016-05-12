#ifndef _OVERLAYCLIENT_H
#define _OVERLAYCLIENT_H
#include "Server.h"
#include "../thread.h"
#include<string>
#include<cstring>
#include<cstdio>

const int MSG_LEN = 1024;
const int CHARS_LEN  = 32;

class OverlayClient {

public:

    OverlayClient(char *ip, int port);
    ~OverlayClient();

    static void *run(void*);
    static void handleConn(char *dstId, char *dstIp, int dstPort);
    static void handleSucc();
    static void handleFailed();
    static int findChars(char *msg);
    void communicationWithServer(); 
    static void initWork();
    void registerToServer();
    static void getDestinationInfo(char *msg);
    
    static void sendMsg(char *msg, int n);
    static void recvMsg(char *buf);
    

private:

    static std::string groupId;
    static std::string destId;

    static std::string destIp;
    static int destPort;

    static std::string srcIp;
    static int srcPort;

    static int sock;
    ClientSocket *client;

};

#endif
