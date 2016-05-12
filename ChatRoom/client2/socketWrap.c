#include<stdio.h>
#include"socketWrap.h"

int Socket() {
    int ret;
    ret = socket(AF_INET, SOCK_STREAM, 0);
    if (ret == -1) {
        sprintf("Socket error: %s\n", strerror(errno));
        return -1;
    }
    return ret;
}

void Connect(int fd, SA* servaddr, socklen_t len) {

    int ret = connect(fd, servaddr, len);
    if (ret == -1) {
        perror("Connect error");
        return;
    }
}


int ClientInit(char *ip, int port) {
    
    int sockfd;
    int ret;
    struct sockaddr_in servaddr;
    sockfd = Socket();
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9999);
    if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
        perror("Init server addr error\n");
        exit(0);
    }

    printf("sockfd: %d\n", sockfd);
    ret = connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    if (ret < 0) {
        printf("ret: %d\n", ret);
        printf("error: %s\n", strerror(errno));
        perror("Connect error");
        exit(0);
    }
    return sockfd;
}


void close_and_disable(int sock, struct epoll_event ee) {
    close(sock);
    ee.data.fd = -1;
}
