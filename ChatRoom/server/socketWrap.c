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

void Bind(int fd, struct sockaddr_in *addr, socklen_t len) {
    
    int ret = bind(fd, (SA*)addr, len);
    if (ret == -1) {
        sprintf("Bind error: %s\n", strerror(errno));
        return;
    }
    //return ret;
}

void Listen(int fd, int backlog) {
    
    int ret = listen(fd, backlog);
    if (ret == -1) {
        sprintf("Listen error: %s\n", strerror(errno));
        return;
    }
    //return ret;, 
}

int Accept(int fd, struct sockaddr_in* addr,socklen_t *len) {
    
    int ret = accept(fd, (SA*)addr, len);
    if (ret == -1) {
        sprintf("Accept error: %s\n", strerror(errno));
        return -1;
    }
    return ret;
}


int ServerInit(int port) {
    
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = Socket();
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    Bind(sockfd, &servaddr, sizeof(servaddr));
    Listen(sockfd, 5);
    return sockfd;
}

int make_socket_noblocking(int fd) {
    
    int flags, s;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("Fcntl error!");
        return -1;
    }
    flags |= O_NONBLOCK;
    s = fcntl(fd, F_GETFL, flags);
    if (s == -1) {
        perror("Fcntl error!");
        return -1;
    }
    return 0;
}

int
create_and_bind (char *port)
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int s, sfd;

  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
  hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
  hints.ai_flags = AI_PASSIVE;     /* All interfaces */

  s = getaddrinfo (NULL, port, &hints, &result);
  if (s != 0)
    {
      fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
      return -1;
    }

  for (rp = result; rp != NULL; rp = rp->ai_next)
    {
      sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (sfd == -1)
        continue;

      s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
      if (s == 0)
        {
          /* We managed to bind successfully! */
          break;
        }

      close (sfd);
    }

  if (rp == NULL)
    {
      fprintf (stderr, "Could not bind\n");
      return -1;
    }

  freeaddrinfo (result);

  return sfd;
}

void close_and_disable(int sock, struct epoll_event ee) {
    close(sock);
    ee.data.fd = -1;
}
