#include<iostream>
#include "Server.h"
#include<errno.h>
#include<time.h>

using namespace std;

ClientSocket::ClientSocket(char *ip, int port) {

    int sock = SocketInIpv4();
    serverPort = port;
    localSock = sock;

    srand((unsigned)time(0));
    int rand_port = rand()%1000 + 1026;

    struct sockaddr_in serv, mine;
    struct sockaddr_in *client = new (struct sockaddr_in);
    socklen_t len;

    bzero(&serv, sizeof(serv));
    bzero(&mine, sizeof(mine));
    bzero(client, sizeof(*client));
//*************************************************
    mine.sin_family = AF_INET;
    mine.sin_port = htons(rand_port);
    mine.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(sock, &mine, sizeof(mine));
//*************************************************
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv.sin_addr) <= 0) {
        cout << "inet_pton error!" << endl;
        exit(-1);
    }

    Connect(sock, &serv, sizeof(serv));

    if (getsockname(sock, (struct sockaddr*)client, &len) != 0) {

        cout << "getsockname error: ";
        cout << strerror(errno) << endl;
        exit(-1);
    }

    localIp = inet_ntoa(client->sin_addr);
    localPort = client->sin_port;
    cout << localIp << "$" << localPort << endl;
    delete client;
}

ClientSocket::~ClientSocket(){
    
    Close(localSock);
}

string ClientSocket::getLocalIp() {
    
    return localIp;
}

int ClientSocket::getLocalPort() {
    
    return localPort;
}

int ClientSocket::getSocket(){
    
    return localSock;
}
