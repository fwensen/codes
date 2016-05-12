#ifndef _SOCKETWRAP_H
#define _SOCKETWRAP_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include<arpa/inet.h>

typedef struct sockaddr SA;

int Socket();
int ClientInit(char*, int port);
void Connect(int, SA*, socklen_t);
#endif
