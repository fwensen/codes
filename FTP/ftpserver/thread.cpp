#include<iostream>
#include"thread.h"
using namespace std;

int FTPThread::client_cmd_sock = -1;
int FTPThread::client_data_sock = -1;
int FTPThread::translate_mode = MODE_BINARY;
int FTPThread::port_mode = PASSIVE_MODE;
struct sockaddr_in FTPThread::client_addr;
struct sockaddr_in FTPThread::server_addr_in_port;
char FTPThread::cmd_buf[CMD_MAXLINE];
char FTPThread::data_buf[DATA_MAXLINE];
char FTPThread::format_buf[CMD_MAXLINE];

void FTPThread::run() {
   
    pthread_t tid;
    int sock = client_cmd_sock;
    int ret = pthread_create(&tid, NULL, doIt, (void*)&sock);
    if (ret != 0) {
        cout << "pthread error" << endl;
        return;
    }
}

void *FTPThread::doIt(void *arg) {
    int fd = *(int *)arg;
    doHandleClient(fd);
}

void FTPThread::doHandleClient(int fd) {
    int loginFlag = doLogin(fd); 
    std::cout << "flag: " << loginFlag << "\n";
    if (loginFlag == AUTH_FAILED)
        return;
    
    char *tmp_buf = (char *)malloc(CMD_MAXLINE*sizeof(char));
    while (myread(fd, tmp_buf, CMD_MAXLINE)){
        
        int cmd = checkCmd(tmp_buf);
        memset(cmd_buf, 0, sizeof(cmd_buf));
        memcpy(cmd_buf, tmp_buf, strlen(tmp_buf));
        switch (cmd) {
            
        case M_SYST:

            rio_writen(fd, (void*)ftp_response_215, strlen(ftp_response_215));
            break;
        case M_FEAT:
            
            rio_writen(fd, (void*)ftp_response_502, strlen(ftp_response_502));
            break;
        case M_PWD:
            {
            char pwd_info[DATA_MAXLINE];
            char tmp[DATA_MAXLINE];
            snprintf(pwd_info, DATA_MAXLINE, "257 \"%s\" is current location\r\n", getcwd(tmp, DATA_MAXLINE));
            rio_writen(fd, (void*)pwd_info, strlen(pwd_info));
            }
            break;

        case M_TYPE_I:
            {
                translate_mode = MODE_BINARY;
                //translate_mode = MODE_BINARY;
                rio_writen(fd, (void *)ftp_response_200, strlen(ftp_response_200));
            }
            break;

        case M_PASV:
            
            port_mode = PASSIVE_MODE;
            //port_mode = PASSIVE_MODE;
            cmd_pasv(fd);
            break;

        case M_CWD:

            cmd_cwd(fd);
            break;

        case M_LIST:

            cmd_list(fd);
            break;

        case M_SIZE:

            cmd_size(fd);
            break;

        case M_MDTM:
            
            cmd_mdtm(fd);
            break;

        case M_RETR:

            cmd_retr(fd);
            break;

        case M_PORT:

            cmd_port(fd);
            break;

        case M_STOR:

            cmd_stor(fd);
            break;

        case M_QUIT:

            Close(fd);
            cout << "a client left!" << endl;
            free(tmp_buf);
            pthread_exit(NULL);
            break;
        default:
            rio_writen(fd, (void*)ftp_response_502, strlen(ftp_response_502));
            break;
        }
    } 
    free(tmp_buf);
}

int FTPThread::doLogin(int fd){
    int clientFlag = -1, flag = AUTH_FAILED;
    rio_writen(fd, (void *)ftp_response_220, strlen(ftp_response_220));

    std::cout << "fd in doLogin: " << fd << "\n";
    std::cout << "after find user\n";
    while (true) {
        if ( myread(fd, cmd_buf, CMD_MAXLINE) > 0)
            break;
        else {
           rio_writen(fd, (void*)ftp_response_502, strlen(ftp_response_502)); 
        }
    }

    int len = strlen(cmd_buf);
    for (int i = 5; i < len; ++i)
        format_buf[i - 5] = cmd_buf[i];
    format_buf[len - 7] = '\0';

    if (strcmp(format_buf, default_user) == 0) {
        rio_writen(fd, (void*)ftp_response_331, strlen(ftp_response_331));    
        clientFlag = 1;
    }

    else if (strcmp(format_buf, anony_user) == 0) {
        rio_writen(fd, (void*)ftp_response_331, strlen(ftp_response_331));
        clientFlag = 2; 
    }    
    
    if (myread(fd, cmd_buf, CMD_MAXLINE) > 0) {
        int len = strlen(cmd_buf);
        for (int i = 5; i < len; ++i)
            format_buf[i - 5] = cmd_buf[i];
        format_buf[len - 7] = '\0';
    } else {
        rio_writen(fd, (void*)ftp_response_421, strlen(ftp_response_421));
        Close(fd);
        return AUTH_FAILED;
    }

    if (clientFlag == 1 && strcmp(format_buf, default_pwd) == 0) {
        rio_writen(fd, (void*)ftp_response_230, strlen(ftp_response_230));
        flag = AUTH_SUCC;
    } else if (clientFlag == 2) {
        flag = AUTH_NOTVERIFY;
        rio_writen(fd, (void*)ftp_response_230, strlen(ftp_response_230));
        std::cout << "anonymous passwd: " << format_buf << "\n";
    }

    return flag;

}

int FTPThread::checkCmd(char *buf) {
    
    if ((strncmp("quit", buf, 4) == 0) || strncmp("QUIT", buf, 4) == 0)
        return M_QUIT;
    else if (strncmp("syst", buf, 4) == 0 || strncmp("SYST", buf, 4) == 0)
        return M_SYST;
    else if(strncmp("feat", buf, 4) == 0 || strncmp("FEAT", buf, 4) == 0)
        return M_FEAT;
    else if (strncmp("pwd", buf, 3) == 0 || strncmp("PWD", buf, 3) == 0)
        return M_PWD;
    else if (strncmp("TYPE I", buf, 6) == 0 || strncmp("type I", buf, 6) == 0)
        return M_TYPE_I;
    else if (strncmp("pasv", buf, 4) == 0 || strncmp("PASV", buf, 4) == 0)
        return M_PASV;
    else if (strncmp("cwd", buf, 3) == 0 || strncmp("CWD", buf, 3) == 0)
        return M_CWD;
    else if (strncmp("list", buf, 4) == 0 || strncmp("LIST", buf, 4) == 0)
        return M_LIST;
    else if (strncmp("size", buf, 4) == 0 || strncmp("SIZE", buf, 4) == 0)
        return M_SIZE;
    else if (strncmp("mdtm", buf, 4) == 0 || strncmp("MDTM", buf, 4) == 0)
        return M_MDTM;
    else if (strncmp("retr", buf, 4) == 0 || strncmp("RETR", buf, 4) == 0)
        return M_RETR;
    else if (strncmp("port", buf, 4) == 0 || strncmp("PORT", buf, 4) == 0)
        return M_PORT;
    else if (strncmp("stor", buf, 4) == 0 || strncmp("STOR", buf, 4) == 0)
        return M_STOR;
    else
        return M_NOTSUPPORT;

}

void FTPThread::cmd_pasv(int fd) {
    
    char pasv_info[CMD_MAXLINE];
    char port_info[8];
    char addr_info[32];

    struct sockaddr_in *data_addr;
    data_addr = create_data_addr();

    std::cout << "port: " << data_addr->sin_port << "\n";

    int port1, port2;
    port1 = ntohs(data_addr->sin_port) / 256;
    port2 = ntohs(data_addr->sin_port) % 256;

    long ip = inet_addr(inet_ntoa(data_addr->sin_addr));
    std::cout << "ip: " << ip << "\n";
    snprintf(addr_info, 32, "%ld,%ld,%ld,%ld,", ip&0xff, ip>>8&0xff,ip>>16&0xff, ip>>24&0xff);

    snprintf(port_info, 8, "%d,%d", port1, port2);
    strcat(addr_info, port_info);
    snprintf(pasv_info, CMD_MAXLINE, "227 Entering passive mode (%s).\r\n", addr_info);
    rio_writen(fd, (void*)pasv_info, strlen(pasv_info));
    std::cout << "pasv msg: " << pasv_info << "\n";
    free(data_addr);

}

void FTPThread::cmd_port(int fd) {
    char ipaddr[CMD_MAXLINE];
    int port;
    memset(ipaddr, 0, sizeof(ipaddr));
    //USERS *user = find_user(fd);

    int len = strlen(cmd_buf);
    for (int i = 5; i < len; ++i)
        format_buf[i - 5] = cmd_buf[i];
    format_buf[len - 7] = '\0';
    
    std::string formatStr(format_buf);
    int i = 0, start = 0, num[6], count = 0;
    while (format_buf[i] != '\0') {
        
        if (formatStr[i] == ',') {
            num[count++] = atoi((char*)(formatStr.substr(start, i - start).c_str()));
            start = i + 1;
        }
        ++i;
    }
    num[count] = atoi((char*)(formatStr.substr(start, i-start).c_str()));
    snprintf(ipaddr, CMD_MAXLINE, "%d.%d.%d.%d", num[0], num[1],num[2], num[3]);
    port = num[4]*256 + num[5];
    
    struct sockaddr_in serv_addr, cliaddr;
    bzero(&serv_addr, sizeof(serv_addr));
    bzero(&cliaddr, sizeof(cliaddr));

    client_data_sock = Socket_in_tcpv4();

    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(SERVER_DATA_PORT);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(client_data_sock, (SA*)&cliaddr, sizeof(cliaddr));

    memcpy((void*)&server_addr_in_port, &cliaddr, sizeof(cliaddr));

    //server bind a addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipaddr, &serv_addr.sin_addr) <= 0) {
        
        std::cout << "addr error!\n";
        return;
    }

    rio_writen(fd, (void*)ftp_response_200, strlen(ftp_response_200));

}

void FTPThread::cmd_list(int fd) {
    
    rio_writen(fd, (void*)ftp_response_125, strlen(ftp_response_125));
    //USERS *user = find_user(fd);
    struct sockaddr_in sockaddr;
    socklen_t len = sizeof(struct sockaddr_in);
    int mConnfd;

    if (port_mode == PORT_MODE) {
        mConnfd = client_data_sock;
        Connect(client_data_sock, (SA*)&server_addr_in_port, sizeof (server_addr_in_port));
    } else {
        mConnfd = Accept(client_data_sock, (SA*)&sockaddr, &len);
    }
    
    char full_dir[CMD_MAXLINE];
    char list_cmd[CMD_MAXLINE];
    snprintf(list_cmd, CMD_MAXLINE, "ls -l %s", getcwd(full_dir, CMD_MAXLINE));

    FILE *pipo;
    if ((pipo = popen(list_cmd, "r")) == NULL) {
        std::cout << "popen error!\n";
        return;
    }

    char c;
    while ((c = fgetc(pipo)) != EOF) {
        std::cout << c;
        rio_writen(mConnfd, (void*)&c, 1);
    }

    rio_writen(fd, (void*)ftp_response_226, strlen(ftp_response_226));
    pclose(pipo);
    Close(mConnfd);
    Close(client_data_sock);

}

void FTPThread::cmd_size(int fd) {
    char file_size[32];
    //USERS *user = find_user(fd);
    struct stat *statbuf = file_information(fd);
    std::cout << "stat buf size: " << statbuf->st_size << "\n";
    snprintf(file_size, 32, "213 %d\r\n", statbuf->st_size);
    free(statbuf);
    std::cout << "size: " << statbuf->st_size << "\n";
    rio_writen(fd, (void*)file_size, strlen(file_size));

}

void FTPThread::cmd_retr(int fd) {
    
    rio_writen(fd, (void*)ftp_response_150, strlen(ftp_response_150));
    //USERS *user = find_user(fd);
    struct sockaddr_in client;
    socklen_t len = sizeof(struct sockaddr_in);
    int mConnfd;

    if (port_mode == PORT_MODE) {  
        mConnfd = client_data_sock;
        Connect(client_data_sock, (SA*)&server_addr_in_port, sizeof(server_addr_in_port));

    } else {

        mConnfd = Accept(client_data_sock, (SA*)&client, &len);

    }

    int length = strlen(cmd_buf);
    for (int i = 5; i < length; ++i)
        format_buf[i - 5] = cmd_buf[i];
    format_buf[length - 7] = '\0';

    FILE *fp;
    int fileFd;

    char t_dir[CMD_MAXLINE];
    char t_file_info[CMD_MAXLINE];
    snprintf(t_file_info, CMD_MAXLINE,"%s/%s", getcwd(t_dir, CMD_MAXLINE), format_buf);
    
    if (strncmp(getcwd(t_dir, CMD_MAXLINE), format_buf, strlen(getcwd(t_dir,CMD_MAXLINE))-1) == 0) {
        fp = fopen(format_buf, "rb");
    } else {
        fp = fopen(t_file_info, "rb");
    }

    if (fp == NULL) {
        std::cout << "open file error!\n";
        char cwd_info[CMD_MAXLINE];
        snprintf(cwd_info, CMD_MAXLINE, "550 %s file open error!\r\n:", format_buf);
        rio_writen(fd, (void*)cwd_info, strlen(cwd_info));
        if (port_mode != PORT_MODE)
            Close(mConnfd);
        Close(client_data_sock);
        return;
    } else {
        
        int t_fileno  = fileno(fp), n;
        memset(data_buf, 0, DATA_MAXLINE);
        while ((n = read(t_fileno, data_buf, DATA_MAXLINE)) > 0) {
            rio_writen(mConnfd, (void*)data_buf, n);
        }
    }
    
    fclose(fp);
    Close(mConnfd);
    Close(client_data_sock);
    rio_writen(fd, (void*)ftp_response_226, strlen(ftp_response_226));
}

void FTPThread::cmd_stor(int fd) {
    rio_writen(fd, (void*)ftp_response_150, strlen(ftp_response_150));

   // USERS *user = find_user(fd);
    struct sockaddr_in client;
    socklen_t len = sizeof(struct sockaddr_in);
    int mConnfd;

    if (port_mode == PORT_MODE) {
        mConnfd = client_data_sock;
        Connect(client_data_sock, (SA*)&server_addr_in_port, sizeof(server_addr_in_port));
    } else {
    
        mConnfd = Accept(client_data_sock, (SA*)&client, &len);
    }
    
    int length = strlen(cmd_buf);
    for (int i = 5; i < length; ++i)
        format_buf[i - 5] = cmd_buf[i];
    format_buf[length - 7] = '\0';

    FILE *fp;
    int fileFd;

    char t_dir[CMD_MAXLINE];
    char t_file_info[CMD_MAXLINE];
    snprintf(t_file_info, CMD_MAXLINE,"%s/%s", getcwd(t_dir, CMD_MAXLINE), format_buf);
    
    if (strncmp(getcwd(t_dir, CMD_MAXLINE), format_buf, strlen(getcwd(t_dir,CMD_MAXLINE))-1) == 0) {
        fp = fopen(format_buf, "ab");
    } else {
        fp = fopen(t_file_info, "ab");
    }

    if (fp == NULL) {
        std::cout << "open file error!\n";
        char cwd_info[CMD_MAXLINE];
        snprintf(cwd_info, CMD_MAXLINE, "550 %s file open error!\r\n:", format_buf);
        rio_writen(fd, (void*)cwd_info, strlen(cwd_info));
        if (port_mode != PORT_MODE)
            Close(mConnfd);
        Close(client_data_sock);
        return;
    } else {
        
        int t_fileno  = fileno(fp), n;
        memset(data_buf, 0, DATA_MAXLINE);
        while ((n = read(mConnfd, data_buf, DATA_MAXLINE)) > 0) {
            rio_writen(t_fileno, (void*)data_buf, n);
        }
    }
    
    fclose(fp);
    Close(mConnfd);
    Close(client_data_sock);
    rio_writen(fd, (void*)ftp_response_226, strlen(ftp_response_226));

}

void FTPThread::cmd_mdtm(int fd) {
    struct stat *statbuf;
    statbuf = file_information(fd);
    char last_miditime[32];
    snprintf(last_miditime, 32, "213 %d\r\n", statbuf->st_mtime);
    free(statbuf);
    rio_writen(fd, (void*)last_miditime, strlen(last_miditime));

}

void FTPThread::cmd_cwd(int fd) {
    
    char cwd_info[CMD_MAXLINE];
    char tmp_dir[CMD_MAXLINE];
    char client_dir[CMD_MAXLINE];
    char t_dir[CMD_MAXLINE];
    int dirlength = -1;
    int length = strlen(cmd_buf);
    int i = 0; 	
    for(i=4; i<length; i++)
	format_buf[i - 4] = cmd_buf[i];
    format_buf[i-6]='\0';

    if(strncmp(getcwd(t_dir, CMD_MAXLINE), format_buf, strlen(getcwd(t_dir, CMD_MAXLINE))-10)!=0) {
   	getcwd(client_dir, CMD_MAXLINE);
   	dirlength=strlen(client_dir);
   	client_dir[dirlength]='/';
    }

    for (i = 4; i < length; i++) 
   	client_dir[dirlength + i - 3] = cmd_buf[i];
    client_dir[dirlength + i - 5]='\0';
		
    if (chdir(client_dir) >= 0) {
    	snprintf(cwd_info, CMD_MAXLINE, "257 \"%s\" is current location.\r\n", getcwd(tmp_dir, CMD_MAXLINE));
    	rio_writen(fd, (void*)cwd_info, strlen(cwd_info));
    } else {
 	snprintf(cwd_info, CMD_MAXLINE, "550 %s :%s\r\n", client_dir, strerror(errno));
  	perror("chdir():");
  	rio_writen(fd, (void*)cwd_info, strlen(cwd_info));
    }    		

}


struct stat *FTPThread::file_information(int fd) {
    
    struct stat *statbuf = (struct stat *)malloc(sizeof(*statbuf));
    //char absolute_dir[CMD_MAXLINE];
    char file_info[CMD_MAXLINE];
    memset(file_info, 0, sizeof(file_info));

    int len = strlen(cmd_buf);
    for (int i = 5; i < len; ++i) 
        format_buf[i - 5] = cmd_buf[i];
    format_buf[len - 7] = '\0';

    snprintf(file_info, CMD_MAXLINE, "%s", format_buf);
    std::cout << "file info: " << file_info << "\n";
    if (stat(file_info, statbuf) == 0) {
        
        std::cout << "stat ok!\n";
    } else {
        
        std::cout << "stat failed: "<< errno << "\n";
    }
    return statbuf;

}


struct sockaddr_in *FTPThread::create_data_addr() {
   
    int t_client_sock;
    struct sockaddr_in *t_data_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    t_client_sock = Socket_in_tcpv4();
    
    srand((int)time(0));
    int a=rand()%1000+1025;
    bzero(t_data_addr, sizeof(*t_data_addr));
    t_data_addr->sin_family = AF_INET;
    t_data_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    t_data_addr->sin_port = htons(a);

        
    Bind(t_client_sock, (SA*)t_data_addr, sizeof(struct sockaddr));
    Listen(t_client_sock, 5);

    client_data_sock = t_client_sock;
    return t_data_addr;

}

int FTPThread::myread(int fd, char *buf, int len) {
    
    int n;
    if((n = recv(fd, buf, len, 0)) < 0) {
        std::cout << "recv error\n";
        return -1;
    }
    buf[n] = '\0';
    std::cout << pthread_self() << "client info: " << buf << "\n";
    return n;
}
