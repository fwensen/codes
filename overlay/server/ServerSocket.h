#ifndef _SERVERSOCKET_H
#define _SERVERSOCKET_H
#include "../socketWrap.h"
//#include<vector>

class ServerSocket {

public:
    
    explicit ServerSocket(int port);
    ~ServerSocket();  

    int ServerAccept();

    int getSockFd();
    int getPort();
    struct sockaddr_in* getAddr();

    int getPeerPort();
    char *getPeerAddr();


private:

//    static vector<int> clients;
    int *serverFd;
    int serverPort;
    struct sockaddr_in *serverAddr;

    int peerPort;
    char *peerAddr;
    


};
#endif
