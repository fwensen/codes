#ifndef _FTP_CLIENT_H
#define _FTP_CLIENT_H
#include "wrap.h"
#include "const.h"
#include<termios.h>
#include<netdb.h>
#include<errno.h>
#include<fcntl.h>

class FTPClient {

public:

    FTPClient();
    FTPClient(char *, int);


private:

    static int sock_control;
    static struct sockaddr_in ftp_server, local_host;
    static struct hostent *server_hostent;
    static char user[64], passwd[64];

    static int mode;

    static struct termios stored_settings;


    static void echo_off();
    static void echo_on();
    static int cmd_err_exit(char *err_msg, int err_code);
    static int fill_host_addr(char*, struct sockaddr_in*, int);
    static int xconnect(struct sockaddr_in*, int type);
    static int ftp_send_cmd(const char *s1, const char *s2, int );
    static int ftp_get_reply(int);
    static int get_port();
    static int rand_port();
    static int xconnect_ftpdata();
    static void ftp_list();
    static void ftp_cmd_filename(char*, char*, char*);
    static void ftp_get(char *);
    static void ftp_put(char*);
    static void ftp_quit();
    static void ftp_pwd();
    static void ftp_cd(char *);
    static void get_user();
    static void get_pass();
    static int ftp_login();
    static int ftp_usr_cmd(char *usr_cmd);
    static void open_srv();
    static int start_ftp_cmd(char*, int);
    static void open_ftpsrv();
    //static int int data_sock;


};


#endif
