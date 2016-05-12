#ifndef _FTP_H
#define _FTP_H
#include "const.h"
#include<string>

class FTP_SERVER {

public:
    
    void openListend(int backlog);

private:
    int server_fd;
    static struct sockaddr_in server_addr;


};
#endif
