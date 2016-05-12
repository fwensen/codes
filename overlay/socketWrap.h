#ifndef _SOCKETWRAP_H
#define _SOCKETWRAP_H
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<cstdlib>
#include<cstdio>
#include<netinet/in.h>
#include<sys/types.h>

int SocketInIpv4();
void Connect(int sockfd, struct sockaddr_in *sockaddr, socklen_t len);
void Bind(int sockfd, struct sockaddr_in *sockaddr, socklen_t len);
void Listen(int sock, int backlog);
int Accept(int sockfd, struct sockaddr_in *sockaddr, socklen_t *len);
void Close(int sockfd);


#endif
