/*************************************************************************
	> File Name: socketWrap.h
	> Author:vincent 
	> Mail: 
	> Created Time: Fri 06 Nov 2015 02:59:23 PM CST
 ************************************************************************/

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

typedef struct sockaddr SA;


int Socket();
void Bind(int, struct sockaddr_in*, socklen_t);
void Listen(int, int);
int Accept(int, struct sockaddr_in*, socklen_t*);
int ServerInit(int port);
int make_socket_noblocking(int fd);
int create_and_bind (char *port);
void close_and_disable(int, struct epoll_event);
#endif
