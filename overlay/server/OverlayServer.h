#ifndef _OVERLAYSERVER_H
#define _OVERLAYSERVER_H
#include<string>
#include<map>
#include<list>
#include"./ServerSocket.h"
#include"../socketWrap.h"

const int CHARLEN = 32;
const int MSGLEN = 1024;

struct GROUP_CLIENTS_ {

    //char name[CHARLEN];
    char dstId[CHARLEN];
    char dstIp[CHARLEN];
    int dstPort;
    char srcIp[CHARLEN];
    int srcPort;
    int sock;
};

struct PEER_SERVER_{

    char id[CHARLEN];
    int sock;
};

//msg 
struct MSG_TYPE_{

    char id[CHARLEN];
    char cmd[CHARLEN];
    char dstId[CHARLEN];
    char dstIp[CHARLEN];
    char srcIp[CHARLEN];
    int dstPort;
    int srcPort;
    char msg[MSGLEN];
};

typedef struct MSG_TYPE_ *MSG_TYPE;
typedef struct GROUP_CLIENTS_ *CLIENTS;
typedef struct PEER_SERVER_ *PEER_SERVER;

class OverLayServer{

public:

    OverLayServer(int port, char *groupId);
    ~OverLayServer();

    //SOCKINFO initLink(char *str, int port, int sockfd);

    static void *recvMsg(void* arg);
    static void sendMsg(char *arg, int sock);
    static void handleMsg(char *arg, int sock);
    static CLIENTS findClient(int sock);
    //static CLIENTS findLocalClient(int port);
    static PEER_SERVER findPeerServer(int sock, char *dstId);

    static char *findSpecificString(char *msg, int m);

    static void handleRequest(char *msg, int sock);
    static void handleMsgFromPeerServer(char *msg, int sock);
    static void handleMsgFromClient(char *msg, int sock);
    static void handleSucc(char *msg, int sock);
    static void handleFailed(char *msg, int sock);
    static void handleConnFromPeerServer(char *msg, int sock);
    static void handleConnToPeer(char *msg, int sock);
    static void handleRegister(char *msg, int sock);
    static void *connectOtherServer(void *arg);


private:

    static char *groupId;

    static std::list<CLIENTS> *groupClients;
    static std::list<PEER_SERVER> *peerServer;
};

#endif
