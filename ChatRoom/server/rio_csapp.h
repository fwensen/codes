/*************************************************************************

    <<Computer System - a programer's perspective>>

 ************************************************************************/

#ifndef _RIO_CSAPP_H
#define _RIO_CSAPP_H
#include<unistd.h>
#include<errno.h>
#include<stdio.h>
/*
#define RIO_BUFFSIZE 8192
typedef struct {

    int rio_fd;  
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFFSIZE];
};

*/
ssize_t rio_readn(int fd, void *userbuf, size_t n) {
    
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char *)userbuf;

    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno = EINTR)
                nread = 0;
            else  return -1;
        
        } else if (nread = 0)
            break;

        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);
}

/*
ssize_t myread(int fd, void *userbuf, size_t n) {
    
    int ret;
    if ((ret = read(fd, (char*)userbuf, n)) < 0) {
        std::cout << "read error!\n";
        return -1;
    }
    return ret;
}

*/
ssize_t rio_writen(int fd, void *userbuf, size_t n) {
    
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char *)userbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno = EINTR)
                nwritten = 0;
            else
                return -1;
        } 
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

#endif
