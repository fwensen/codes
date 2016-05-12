#include<iostream>
#include"epollEvent.h"
using namespace std;

struct epoll_event EpollEvent::event;
struct epoll_event EpollEvent::events[MAXEVENTS];
int EpollEvent::eventFd;

EpollEvent::EpollEvent(int port){

    initServer(port);
    createEvent();
    addEvent(servsock);
}

void EpollEvent::createEvent(){

    eventFd = epoll_create(256);
}

void EpollEvent::addEvent(int sock) {
    
    event.data.fd = sock;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(eventFd, EPOLL_CTL_ADD, sock, &event);

}

int EpollEvent::waitEvent() {
    
    int nfds = epoll_wait(eventFd, events, MAXEVENTS, -1);
    return nfds;
}

void EpollEvent::doWork() {
    
    bool bOut = false;
    int i;
    int connfd, sockfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    bzero(&cliaddr, sizeof(cliaddr));

    bool isReadCompleted = false;
    for (; ;) {
        if (bOut)
            break;
        int nfds = waitEvent();
        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == servsock) {
                connfd = accept(servsock, (SA*)&cliaddr, &clilen);
                if (connfd < 0) {
                    cout << "Accept error!\n";
                    return;
                }

                cout << "Got a connection!\n";
                make_socket_noblocking(connfd);
                addEvent(connfd);
                //addUserRegister(connfd, false);

            } else if (events[i].events & EPOLLIN) {
                
                if ((sockfd = events[i].data.fd) < 0)
                    continue;

                int recvNUm = 0, count = 0;
                char recvbuf[1024];
                memset(recvbuf, 0, sizeof(recvbuf));

                while (true) {
                    
                    recvNUm = recv(sockfd, recvbuf + count, 20, 0);
                    if (recvNUm < 0) {
                        if (errno == EAGAIN) {
                            isReadCompleted = true;
                            break;
                        } else if (errno == ECONNRESET) {
                            close_and_disable(sockfd, events[i]);
                            cout << "Counterpart ased RST\n";
                            break;
                        } else if (errno == EINTR) 
                            continue;
                        else {
                            close_and_disable(sockfd, events[i]);
                            cout << "Unknown Error\n";
                            break;
                        }
                    } else if (recvNUm == 0) {
                        close_and_disable(sockfd, events[i]);
                        cout << "Counterpart had shuted off!\n";
                        removeUser(events[i].data.fd);
                        break;
                    }

                    count += recvNUm;
                    if (recvNUm == 20)
                        continue;
                    else {
                        isReadCompleted = true;
                        break;
                    }
                }

                if (isReadCompleted) {
                    recvbuf[count] = '\0';
                    cout << "Recv: " << recvbuf;
                    //判断用户是否注册，如果没有，为注册信息，将其加入群组.
                    if (!isUserRegister(events[i].data.fd)) {
                       addUser(recvbuf, events[i].data.fd);
                       cout << "Add\n";
                    }
                    else {    //注册完成，则群发信息
                       cout << "Send\n";
                       sendGroupMsg(recvbuf); 

                    }

                }
            }
        }

    }

}
