#ifndef _WRAP_H
#define _WRAP_H

#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<cstring>
#include<string>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<time.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<netinet/in.h>


typedef struct _SIMPLE_USER SIMPLE_USER;

struct _SIMPLE_USER{
    int sock;
    struct sockaddr_in client;
};

typedef struct sockaddr SA;


int Socket_in_tcpv4() {

    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if (ret == -1) {
        std::cout << "socket init error!\n";
    }
    return ret;
}

int Connect(int fd, const SA *serveraddr, socklen_t addrlen) {
    
    int ret = connect(fd, serveraddr, addrlen);
    if (ret == -1) {
        std::cout << "Connect error!\n";
    }
    return ret;
}

int Bind(int fd, const SA *myaddr, socklen_t addrlen) {
        
    int ret = bind(fd, myaddr, addrlen);
    if (ret == -1) {

        std::cout << "Bind error!\n";
    }
    return ret;
}

int Listen(int fd, int backlog) {
    
    int ret = listen(fd, backlog);
    if (ret == -1) {
        std::cout << "Listen error!\n";
    }
}

int Accept(int fd, SA *cliaddr, socklen_t *addrlen) {
    
    int ret = accept(fd, cliaddr, addrlen);
    if (ret == -1) {
        std::cout << "Accept error!\n";
    }
    return ret;
}

void Close(int fd) {
    
    int ret = close(fd);
    if (ret == -1) {
        std::cout << "Close error!\n";
        return;
    }
}

void Shutdown_WR(int fd) {
    
    int ret = shutdown(fd, SHUT_WR);
    if (ret == -1) {
        std::cout << "SHutdown error!\n";
        return;
    }
}


#endif
