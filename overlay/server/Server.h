#ifndef _SERVER_H
#define _SERVER_H
#include"../socketWrap.h"
#include<string>


class ClientSocket {

public:

    ClientSocket(char *ip, int port);
    ~ClientSocket();
    //int Connect();
    int getSocket();
    std::string getLocalIp();
    int getLocalPort();


private:

    int localSock;
    int serverPort;

    std::string localIp;
    int localPort;

};
#endif
