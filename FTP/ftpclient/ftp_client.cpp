/*************************************************************************
	> File Name: ftp_client.cpp
	> Author: Vincent 
	> Mail: 
 ************************************************************************/
#include<iostream>
#include "ftp_client.h"
using namespace std;

int FTPClient::sock_control = -1;
struct sockaddr_in FTPClient::ftp_server;
struct sockaddr_in FTPClient::local_host;
struct hostent *FTPClient::server_hostent;
char FTPClient::user[64];
char FTPClient::passwd[64];
struct termios FTPClient::stored_settings;

//int FTPClient::data_sock = -1;
int FTPClient::mode = PASSIVE_MODE;

FTPClient::FTPClient() {

    open_ftpsrv();
}

FTPClient::FTPClient(char *ip, int port) {
    
    start_ftp_cmd(ip, port);
}

void FTPClient::echo_off(){
    
    struct termios new_settings;
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ECHO);
    tcsetattr(0, TCSANOW, &new_settings);
    return;
}

void FTPClient::echo_on(){
    
    tcsetattr(0, TCSANOW, &stored_settings);
    return;
}

int FTPClient::cmd_err_exit(char *err_msg, int err_code) {
    cout << err_msg << endl;
    return err_code;
}

int FTPClient::fill_host_addr(char *host_ip_addr, struct sockaddr_in *host, int port) {

    if (port <= 0 || port > 65535)
        return 254;
    bzero(host, sizeof(struct sockaddr_in));
    host->sin_family = AF_INET;
    if (inet_addr(host_ip_addr) != -1)
        host->sin_addr.s_addr = inet_addr(host_ip_addr);
    else {
        if ((server_hostent = gethostbyname(host_ip_addr)) != 0) {
            memcpy(&host->sin_addr, server_hostent->h_addr, sizeof(host->sin_addr));
        } else
            return 253;
    }
    host->sin_port = htons(port);
}

int FTPClient::xconnect(struct sockaddr_in *s_addr, int type) {
    
    struct timeval outtime;
    int set;
    int sock = Socket_in_tcpv4();

    outtime.tv_sec = 1;
    outtime.tv_usec = 0;

    set = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &outtime, sizeof(outtime));
    if (set != 0) {
        cout << "set sockopt error" << endl;
        cmd_err_exit("exit socket", 1);
    } 

    Connect(sock, (SA*)s_addr, sizeof(struct sockaddr_in));
    return sock;
}

int FTPClient::ftp_send_cmd(const char *s1, const char *s2, int sock) {
    
    char send_buf[512];
    int send_err, len;
    if (s1) {

        strcpy(send_buf, s1);
        if (s2) {

            strcat(send_buf, s2);
            strcat(send_buf, "\r\n");
            len = strlen(send_buf);
            send_err = send(sock, send_buf, len, 0);
        } else {
            strcat(send_buf, "\r\n");
            len = strlen(send_buf);
            send_err = send(sock, send_buf, len, 0);
        }
        cout << "send msg: " << send_buf;
    }

    if (send_err < 0)
        cout << "send error!\n";
    return send_err;
}

int FTPClient::ftp_get_reply(int sock) {
    
    char buf[512];
    int count = read(sock, buf, 510), reply_code;
    if (count > 0)
        reply_code = atoi(buf);
    else
        return 0;
    while(true) {
        if (count <= 0)
            break;
        buf[count] = '\0';
        cout << buf;
        count = read(sock, buf, 510);
    }
    return reply_code;
}

int FTPClient::get_port() {
    
    char response[512];
    int port_num, count;
    char *ptr;
    memset(response, 0, sizeof(response));
    ftp_send_cmd("PASV", NULL, sock_control);
    count = read(sock_control, response, 510);
    response[count] = '\0';
    if (count <= 0)
        return 0;
    if (atoi(response) == 227) {
        ptr = strrchr(response, ',');
        port_num = atoi(ptr + 1);
        *ptr = '\0';
        ptr = strrchr(response, ',');
        port_num += atoi(ptr + 1) * 256;
        return port_num;
    }
    return 0;
}

int FTPClient::rand_port() {
    
    int port;
    srand((unsigned)time(NULL));
    port = rand() % 40000 + 1025;
    return port;
}

int FTPClient::xconnect_ftpdata() {
   
    if (mode == PASSIVE_MODE) {
        int port = get_port();
        ftp_server.sin_port = htons(port);
        return (xconnect(&ftp_server, 0));
    } else {
        
            int client_port, get_sock, opt, set;
            char cmd_buf[32];
            struct timeval outtime;
            struct sockaddr_in local;
            char local_ip[32];
            char *ip_1, *ip_2, *ip_3, *ip_4;
            int addr_len = sizeof(struct sockaddr_in);
            client_port = rand_port();
            get_sock = Socket_in_tcpv4();
            if (get_sock < 0)
                cmd_err_exit("socket", 1);

            outtime.tv_sec = 7;
            outtime.tv_usec = 0;
            opt = SO_REUSEADDR;
            set = setsockopt(get_sock, SOL_SOCKET, SO_RCVTIMEO, &outtime, sizeof(outtime));
        if (set != 0) {
                cout << "setsockopt error!\n";
                cmd_err_exit("setsockopt", 1);
            }

            set = setsockopt(get_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            if (set != 0) {
                cout << "setsockopt error\n";
                cmd_err_exit("setsockopt", 1);
            }

            bzero(&local_host, sizeof(local_host));
            local_host.sin_family = AF_INET;
            local_host.sin_port = htons(client_port);
            local_host.sin_addr.s_addr = htonl(INADDR_ANY);
            bzero(&local, sizeof(SA));
            while (true) {
                
                set = Bind(get_sock, (SA*)&local_host, sizeof(local_host));
                set = Listen(get_sock, 5);
                if (getsockname(sock_control, (SA*)&local, (socklen_t*)&addr_len ) < 0)
                    return -1;
                snprintf(local_ip, sizeof(local_ip), inet_ntoa(local.sin_addr));
                local_ip[strlen(local_ip)] = '\0';
                ip_1 = local_ip;
                ip_2 = strrchr(local_ip, '.');
                *ip_2 = '\0';
                ip_2++;
                ip_3 = strrchr(ip_2, '.');
                *ip_3 = '\0';
                ip_3++;
                ip_4 = strrchr(ip_3, '.');
                *ip_4 = '\0';
                ip_4++;
                snprintf(cmd_buf, sizeof(cmd_buf), "PORT %s,%s,%s,%s,%d,%d", ip_1, ip_2, ip_3, ip_4, client_port>>8, client_port&0xff);
                ftp_send_cmd(cmd_buf, NULL, sock_control);
                if (ftp_get_reply(sock_control) != 200) {
                    cout << "can not use port mode!\n";
                    return -1;
                }
                else
                    return get_sock;
            }
    }
}

void FTPClient::ftp_list() {
    
    int i = 0, new_sock;
    int set = sizeof(local_host);
    int list_sock = xconnect_ftpdata();
    if (list_sock < 0) {
        ftp_get_reply(sock_control);
        cout << "create data sock error!\n";
        return;
    }
    ftp_get_reply(sock_control);
    ftp_send_cmd("LIST", NULL, sock_control);
    ftp_get_reply(sock_control);
    if (mode == PASSIVE_MODE) {
        ftp_get_reply(list_sock);
    } else {

        while (i < 3) {
            new_sock = Accept(list_sock, (SA*)&local_host, (socklen_t*)&set);
            if (new_sock == -1) {
                cout << "Accept error!\n";
                i++;
            } else
                break;
        }

        if (new_sock == -1) {
            cout << "You can not use port mode\n";
            return;
        }
        ftp_get_reply(new_sock);
        Close(new_sock);
    }

    Close(list_sock);
    ftp_get_reply(sock_control);
}


void FTPClient::ftp_cmd_filename(char *usr_cmd, char *src_file, char *dst_file) {

    int length, flag = 0;
    int i = 0, j = 0;
    char *cmd_src;
    cmd_src = strchr(usr_cmd, ' ');
    if (!cmd_src) {
        cout << "cmd error!\n";
        return;
    } else {
        while (*cmd_src == ' ')
            cmd_src++;

    }

    if (cmd_src == NULL || cmd_src == '\0')
        cout << "cmd error\n";
    else {
        
        length = strlen(cmd_src);
        while(i <= length) {
            if (*(cmd_src + i) != ' ' && *(cmd_src + i) != '\\') {
                if (flag == 0)
                    src_file[j] = *(cmd_src + i);
                else
                    dst_file[j] = *(cmd_src + i);
                j++;
            }

            if (*(cmd_src + i) == '\\' && *(cmd_src + i + 1) != ' ') {
                if (flag == 0)
                    src_file[j] = *(cmd_src + i);
                else
                    dst_file[j] = *(cmd_src + i);
                j++;
            }

            if (*(cmd_src + i) == ' ' && *(cmd_src + i - 1) != '\\') {
                src_file[j] = '\0';
                flag = 1;
                j = 0;
            }

            if (*(cmd_src + i) == '\\' && *(cmd_src + i + 1) == ' ' ) {
                if (flag == 0)
                    src_file[j] = ' ';
                else
                    dst_file[j] = ' ';
                j++;
            }
            i++;
        }

    }
    if (flag == 0)
        strcpy(dst_file, src_file);
    else
        dst_file[j] = '\0';

}

void FTPClient::ftp_get(char *usr_cmd) {
    
    int get_sock, set, new_sock, i = 0;
	char src_file[512];
	char dst_file[512];
	char rcv_buf[512];
	char cover_flag[3];
	struct stat file_info;
	int local_file;
	int count = 0;
	ftp_cmd_filename(usr_cmd, src_file, dst_file);
	ftp_send_cmd("SIZE ", src_file, sock_control);
	if(ftp_get_reply(sock_control) != 213)
	{
		printf("SIZE error!\n");
		return;
	}
	if(!stat(dst_file, &file_info))
	{
		printf("local file %s exists: %d bytes\n", dst_file, (int)file_info.st_size);
		printf("Do you want to cover it? [y/n]");
		fgets(cover_flag, sizeof(cover_flag), stdin);
		fflush(stdin);
		if(cover_flag[0] != 'y')
		{
			printf("get file %s aborted.\n", src_file);
			return;
		}
	}
	local_file = open(dst_file, O_CREAT|O_TRUNC|O_WRONLY);
	if(local_file < 0)
	{
		printf("creat local file %s error!\n", dst_file);
		return;
	}
	get_sock = xconnect_ftpdata();
	if(get_sock < 0)
	{
		printf("socket error!\n");
		return;
	}
	set = sizeof(local_host);
	
	ftp_send_cmd("TYPE I", NULL, sock_control);
	ftp_get_reply(sock_control);
	ftp_send_cmd("RETR ", src_file, sock_control);
	if(mode == PORT_MODE)
	{
		while(i < 3)
		{
			new_sock = accept(get_sock, (struct sockaddr *)&local_host, \
				(socklen_t *)&set);
			if(new_sock == -1)
			{
				printf("accept return:%s errno: %d\n", strerror(errno),errno);
				i++;
				continue;
			}
				else break;
		}
		if(new_sock == -1)
		{
			printf("Sorry, you can't use PORT mode. There is something wrong when the server connect to you.\n");
			return;
		}
		ftp_get_reply(sock_control);
		while(1)
		{
			printf("loop \n");
			count = read(new_sock, rcv_buf, sizeof(rcv_buf));
			if(count <= 0)
				break;
			else
			{
				write(local_file, rcv_buf, count);
			}
		}
		close(local_file);
		close(get_sock);
		close(new_sock);
		ftp_get_reply(sock_control); 
	}
	else
	{
		ftp_get_reply(sock_control);
		while(1)
		{
			count = read(get_sock, rcv_buf, sizeof(rcv_buf));
			if(count <= 0)
				break;
			else
			{
				write(local_file, rcv_buf, count);
			}
		}
	close(local_file);
	close(get_sock);
	ftp_get_reply(sock_control); 
	}
	if(!chmod(src_file, 0644))
	{
		printf("chmod %s to 0644\n", dst_file);
		return;
	}
	else
		printf("chmod %s to 0644 error!\n", dst_file);
	ftp_get_reply(sock_control); 

}

void FTPClient::ftp_put(char *usr_cmd) {

   char src_file[512];
	char dst_file[512];
	char send_buf[512];
	struct stat file_info;
	int local_file;
	int file_put_sock, new_sock, count = 0, i = 0;
	int set = sizeof(local_host);
	ftp_cmd_filename(usr_cmd, src_file, dst_file);
	if(stat(src_file, &file_info) < 0)
	{
		printf("local file %s doesn't exist!\n", src_file);
		return;
	}
	local_file = open(src_file, O_RDONLY);
	if(local_file < 0)
	{
		printf("open local file %s error!\n", dst_file);
		return;
	}
	file_put_sock = xconnect_ftpdata();
	if(file_put_sock < 0)
	{	
		ftp_get_reply(sock_control);
		printf("creat data sock errro!\n");
		return;
	}
	ftp_send_cmd("STOR ", dst_file, sock_control);
	ftp_get_reply(sock_control);
	ftp_send_cmd("TYPE I", NULL, sock_control);
	ftp_get_reply(sock_control);
	if(mode == PORT_MODE)
	{
		while(i < 3)
		{
			new_sock = accept(file_put_sock, (struct sockaddr *)&local_host, \
				(socklen_t *)&set);
			if(new_sock == -1)
			{
				printf("accept return:%s errno: %d\n", strerror(errno),errno);
				i++;
				continue;
			}
				else break;
		}
		if(new_sock == -1)
		{
			printf("Sorry, you can't use PORT mode. There is something wrong when the server connect to you.\n");
			return;
		}
		while(1)
		{
			count = read(local_file, send_buf, sizeof(send_buf));
			if(count <= 0)
				break;
			else
			{
				write(new_sock, send_buf,sizeof(send_buf));
			}
		}
		close(local_file);
		close(file_put_sock);
		close(new_sock);
	}
	else
	{
		while(1)
		{
			count = read(local_file, send_buf, sizeof(send_buf));
			if(count <= 0)
				break;
			else
			{
				write(file_put_sock, send_buf,count);
			}
		}
		close(local_file);
		close(file_put_sock);
	}
	ftp_get_reply(sock_control); 

}

void FTPClient::ftp_quit(){

    ftp_send_cmd("QUIT",NULL,sock_control);
    ftp_get_reply(sock_control);
    close(sock_control);
}

void FTPClient::ftp_pwd() {

    ftp_send_cmd("PWD", NULL, sock_control);
    ftp_get_reply(sock_control);
}

void FTPClient::ftp_cd(char * usr_cmd)
{
	char *cmd = strchr(usr_cmd, ' ');
	char path[1024];
	if(cmd == NULL)
	{
		printf("command error!\n");
		return;
	}
	else
	{
		while(*cmd == ' ')
			cmd ++;
	}
	if(cmd == NULL || cmd == '\0')
	{
		printf("command error!\n");
		return;
	}
	else
	{
		strncpy(path, cmd, strlen(cmd));
		path[strlen(cmd)]='\0';
		ftp_send_cmd("CWD ", path, sock_control);
		ftp_get_reply(sock_control);
	}
}


void FTPClient::get_user()
{
	char read_buf[64];
	printf("User(Press  for anonymous): ");
	fgets(read_buf, sizeof(read_buf), stdin);
	if(read_buf[0]=='\n')
		strncpy(user, "anonymous", 9);
	else
		strncpy(user, read_buf, strlen(read_buf)-1);
}

void FTPClient::get_pass()
{
	char read_buf[64];
	printf("Password(Press  for anonymous): ");
	echo_off();
	fgets(read_buf, sizeof(read_buf), stdin);
	if(read_buf[0]=='\n')
		strncpy(passwd, "anonymous", 9);
	else
		strncpy(passwd, read_buf, strlen(read_buf)-1);
	echo_on();
	printf("\n");
}

int FTPClient::ftp_login()
{
	int err;
	get_user();
	if(ftp_send_cmd("USER ", user, sock_control) < 0)
		cmd_err_exit("Can not send message",1);
	err = ftp_get_reply(sock_control);
	if(err == 331)
	{
		get_pass();
		if(ftp_send_cmd("PASS ", passwd, sock_control) <= 0)
			cmd_err_exit("Can not send message",1);
		else
			err = ftp_get_reply(sock_control);

		if(err == 230 || err == 531)
			return 1;
		else
		{
			cout << "Password error!\n";
			return 0;
		}
		
	}
	else
	{
		cout << "User error!\n";
		return 0;
	}
}

int FTPClient::ftp_usr_cmd(char * usr_cmd)
{

	if(!strncmp(usr_cmd,"open",4))
		return 15;
	if(!strncmp(usr_cmd,"close",5))
		return 16;
	if(!strncmp(usr_cmd,"mkdir",5))
		return 17;
	if(!strncmp(usr_cmd,"rmdir",5))
		return 18;
	if(!strncmp(usr_cmd,"dele",4))
		return 19;

	if(!strncmp(usr_cmd,"ls",2))
		return 1;
	if(!strncmp(usr_cmd,"pwd",3))
		return 2;
	if(!strncmp(usr_cmd,"cd ",3))
		return 3;
	if(!strncmp(usr_cmd,"put ",4))
		return 4;
	if(!strncmp(usr_cmd,"get ",4))
		return 5;
	if(!strncmp(usr_cmd,"bye",3))
		return 6;
	if(!strncmp(usr_cmd,"mode",4))
		return 7;
	return -1;
}


int FTPClient::start_ftp_cmd(char * host_ip_addr, int port) 
{
	int err;
	int cmd_flag;
	char usr_cmd[1024];
	err = fill_host_addr(host_ip_addr, &ftp_server, port);
	if(err == 254)
		cmd_err_exit("Invalid port!",254);
	if(err == 253)
		cmd_err_exit("Invalid server address!",253);

	sock_control = xconnect(&ftp_server,0);
	if((err =  ftp_get_reply(sock_control)) != 220)
	cmd_err_exit("Connect error!",220);
	err = ftp_login();
    if (err != 1) {
        cout << "login error!\n";
        return -2;
    }
	while(1)
	{
		printf("ftp> ");
		fgets(usr_cmd, 510, stdin);
		fflush(stdin);
		if(usr_cmd[0] == '\n')
			continue;
		usr_cmd[strlen(usr_cmd)-1] = '\0';
        cout << "usr_cmd: " << usr_cmd << endl;
		cmd_flag = ftp_usr_cmd(usr_cmd);
		switch(cmd_flag)
		{
			case 1:
				ftp_list();
				memset(usr_cmd, '\0',sizeof(usr_cmd));
				break;
			case 2:
                cout << "ftp_pwd \n";
				ftp_pwd();
				memset(usr_cmd, '\0',sizeof(usr_cmd));
				break;
			case 3:
				ftp_cd(usr_cmd);
				memset(usr_cmd, '\0',sizeof(usr_cmd));
				break;
			case 4:
				ftp_put(usr_cmd);
				memset(usr_cmd, '\0',sizeof(usr_cmd));
				break;
			case 5:
				ftp_get(usr_cmd);
				memset(usr_cmd, '\0',sizeof(usr_cmd));
				break;
			case 6:
				ftp_quit();
				printf("BYE TO WEILIQI FTP!\n");
				exit(0);
			case 7:
				if (mode == PASSIVE_MODE) { mode = PORT_MODE; cout << "change to port mode" << endl; } 
                else  { mode = PASSIVE_MODE; cout << "change to PASSIVE mode\n"; }
				break;
		}
	}
	return 1;
}

void FTPClient::open_ftpsrv()
{
	char usr_cmd[1024];
	int cmd_flag;
	while(1)
	{
		printf("ftp> ");
		fgets(usr_cmd,510,stdin);
		fflush(stdin);
		if(usr_cmd[0] == '\n')
			continue;
		usr_cmd[strlen(usr_cmd)-1] = '\0';
		cmd_flag = ftp_usr_cmd(usr_cmd);
		if(cmd_flag == 15)
		{
			char *cmd = strchr(usr_cmd,' ');
			char dress_ftp[1024];
			if(cmd == NULL)
			{
				printf("command error!\n");
				return;
			}
			else
			{
				while(*cmd == ' ')
					cmd++;
			}
			if(cmd == NULL||cmd == '\0')
			{
				printf("command error!\n");
				return;
			}
			else
			{
				strncpy(dress_ftp,cmd,strlen(cmd));
				dress_ftp[strlen(cmd)] = '\0';
				printf("Connect Seccessed!\n");
				start_ftp_cmd(dress_ftp, SERVER_PORT);
			}
		}
		else
		{
            cout << "cmd error, open server first!\n";
		}
	}		
		
}



int main(int argc, char *argv[]){
    

    if (argc == 1)
        FTPClient client;
    else
        FTPClient client(argv[1], SERVER_PORT);
}

