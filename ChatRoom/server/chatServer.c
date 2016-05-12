#include<stdio.h>
#include<stdlib.h>
#include"socketWrap.h"
#define MAXEVENTS 1000
#define MAXLINE 1024

int main(int argc, char *argv[]) {

    if (argc != 2) {

        printf("Wrong input!\n");
        return -1;
    }

    int j;
    int epfd, maxi, nfds, i, clilen, connfd, sockfd;
    int count, recvNUm, isReadComplete;
    char recvbuf[MAXLINE];

    int client_socks[MAXEVENTS], socket_count = 0;
    memset(client_socks, -1, sizeof(client_socks));
    memset(recvbuf, 0, sizeof(recvbuf));
    struct sockaddr_in cliaddr;
    clilen = sizeof(cliaddr);
    bzero(&cliaddr, sizeof(cliaddr));
    int servsock;
    servsock = ServerInit(atoi(argv[1]));

    struct epoll_event ev, events[MAXEVENTS];
    epfd = epoll_create(256);
    ev.data.fd = servsock;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, servsock, &ev);

    maxi = 0;
    int bOut = 0;
    for (; ;) {
        
        if (bOut == 1) 
            break;
        nfds = epoll_wait(epfd, events, MAXEVENTS, -1);

        for (i = 0; i < nfds; i++) {
            if (events[i].data.fd == servsock) {
                connfd = accept(servsock, (SA*)&cliaddr, &clilen);
                if (connfd < 0) {
                    perror("Accept error!\n");
                    return -1;
                }

                printf("Recv a connection....!\n");
                make_socket_noblocking(connfd);
                ev.data.fd = connfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
                client_socks[socket_count++] = connfd;
            } else if (events[i].events & EPOLLIN) {
                
                if ((sockfd = events[i].data.fd) < 0) 
                    continue;
                
                recvNUm = 0;
                count = 0;
                isReadComplete = 0;
                while (1) {
                    
                    recvNUm = recv(sockfd, recvbuf + count, MAXLINE, 0);
                    if (recvNUm < 0) {
                        if (errno == EAGAIN) {
                            isReadComplete = 1;
                            break;
                        } else if (errno == ECONNRESET) {
                            close_and_disable(sockfd, events[i]);
                            printf("Counterpart send out RST\n");
                            break;
                        } else if (errno == EINTR) {
                            continue;
                        } else {

                            close_and_disable(sockfd, events[i]);
                            printf("Unknown Error!\n");
                            break;
                        }
                    } else if (recvNUm == 0) {
                        close_and_disable(sockfd, events[i]);
                        printf("Counterpart had shuted off!\n");
                        break;
                    }

                    count += recvNUm;
                    if (recvNUm == MAXLINE)
                        continue;
                    else {
                        isReadComplete = 1;
                        break;
                    }
                }
                if (isReadComplete) {
                    recvbuf[count] = '\0';
                    printf("recv: %s\n", recvbuf);

                    for (j = 0; j < socket_count; j++){
                        write(client_socks[j], recvbuf, strlen(recvbuf));
                    } 
            
                    }

            }
        }

    }
    close(servsock);
}

