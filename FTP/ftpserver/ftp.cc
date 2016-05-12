#include "ftp.h"
#include "rio_csapp.h"
#include "thread.h"
#include"thread.cpp"

struct sockaddr_in FTP_SERVER::server_addr;

void FTP_SERVER::openListend(int backlog) {


    pthread_t tid;

    //struct sockaddr_in servaddr;
    server_fd = Socket_in_tcpv4();
    
    int opt = SO_REUSEADDR;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,&opt, sizeof(opt));

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(server_fd, (SA*)&server_addr, sizeof(server_addr));
    Listen(server_fd, backlog);

    for (; ;) {


        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        int connfd = Accept(server_fd, (SA*) &cliaddr, &clilen);
        SIMPLE_USER user;
        user.sock = connfd;
        memcpy((void*)&user.client, &cliaddr, sizeof(cliaddr));
        FTPThread thread(&user);
        thread.run();
        // pthread_create(&tid, NULL, doIt, (void *)&user);

    }
}

int main(){
    
    FTP_SERVER server;
    server.openListend(5);

}
