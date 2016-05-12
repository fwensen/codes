#ifndef _THREAD_H
#define _THREAD_H
#include"wrap.h"
#include"rio_csapp.h"
#include"const.h"
typedef SIMPLE_USER* S_USER;

class FTPThread {

public:
    
    FTPThread(S_USER user) {

        client_cmd_sock = user->sock;
        memcpy((void*)&client_addr, &user->client, sizeof(user->client));
    }

    static void run();
    
private:

    static int client_cmd_sock;
    static int client_data_sock;
    static int translate_mode;
    static int port_mode;
    static struct sockaddr_in client_addr;
    static struct sockaddr_in server_addr_in_port;
    static char cmd_buf[CMD_MAXLINE];
    static char data_buf[DATA_MAXLINE];
    static char format_buf[CMD_MAXLINE];

private:

    static void *doIt(void *arg);
    static void doHandleClient(int fd);
    static int doLogin(int fd);
    static int checkCmd(char *buf);

    static void cmd_pasv(int);
    static void cmd_port(int);
    static void cmd_list(int);
    static void cmd_size(int);
    static void cmd_retr(int);
    static void cmd_stor(int);
    static void cmd_mdtm(int);
    static void cmd_cwd(int);

    static struct stat *file_information(int);
    static struct sockaddr_in *create_data_addr();
    static int myread(int, char*, int);
};

#endif
