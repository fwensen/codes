#include<iostream>
#include"./ServerSocket.h"
using namespace std;

//int Server::serverFd;
//int Server::serverPort;
//struct sockaddr_in *Server::serverAddr;
//vector<int> Server::clients;

ServerSocket::ServerSocket(int port)  {
    
    serverFd = new int;
    serverPort = port;

    //struct sockaddr_in servaddr;
    *serverFd = SocketInIpv4();
    /*
    *serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (*serverFd == -1) {
        cout << "socket error!\n";
        exit(-1);
    }
    */
    cout << "server fd: " << *serverFd << "port: " << serverPort << endl;

    int opt = SO_REUSEADDR;
    setsockopt(*serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server;
    serverAddr = new struct sockaddr_in;
    peerAddr = new char[24];

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    
    Bind(*serverFd, &server, sizeof(server));
    Listen(*serverFd, 10);

    strncpy((char*)serverAddr, (char*)&server, sizeof(server));
}

ServerSocket::~ServerSocket() {
    Close(*serverFd);
    delete serverAddr;
    delete [] peerAddr;
    delete serverFd;
}

int ServerSocket::ServerAccept() {
    
    int connfd;
    struct sockaddr_in client;

    socklen_t socklen;
    connfd = Accept(*serverFd, &client, &socklen);

    peerPort = client.sin_port;
    cout <<  "Connection from: " << inet_ntoa(client.sin_addr) << "#" << peerPort << endl;
    return connfd;
}

int ServerSocket::getPeerPort(){
    return peerPort;
}

char *ServerSocket::getPeerAddr() {
    return peerAddr;
}

int ServerSocket::getPort() {

    return serverPort;
}

int ServerSocket::getSockFd() {
    
    return *serverFd;
}
