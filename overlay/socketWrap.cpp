#include<iostream>
#include"./socketWrap.h"
#include<errno.h>
#include<string.h>

using namespace std;

int SocketInIpv4() {

    int ret;
    ret = socket(AF_INET, SOCK_STREAM, 0);
    if (ret == -1) {
        cout << "SocketInIpv4 error!" << endl;
        exit(-1);
    }
    return ret;
}

void Connect(int sockfd, struct sockaddr_in *sockaddr, socklen_t len) {
    
    if ( connect(sockfd, (struct sockaddr*)sockaddr, len) == -1 ) {
        
        Close(sockfd);
        cout << "Connect error!" << endl;
        return;
    }
}

void Bind(int sockfd, struct sockaddr_in *sockaddr, socklen_t len) {
    
    if (bind(sockfd, (struct sockaddr*)sockaddr, len) == -1) {
        cout << "Bind error" << endl;
        Close(sockfd);
        return;
    }
}

void Listen(int sockfd, int backlog) {
    
    if (listen(sockfd, backlog) == -1) {

        Close(sockfd);
        cout << "Listen error" << endl;
        return;
    }
}

int Accept(int sockfd, struct sockaddr_in *sockaddr, socklen_t *len) {
    
    int ret;
    if ((ret = accept(sockfd, (struct sockaddr*)sockaddr, len)) == -1) {

        cout << "Accept error:" << strerror(errno) << endl;
        exit(-1);
    }
    return ret;
}

void Close(int sockfd) {
    if (close(sockfd) == -1) {

        cout << "Close error" << endl;
        return;
    }
}

