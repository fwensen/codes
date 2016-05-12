#include<iostream>
#include<stdlib.h>
#include<cstring>
#include<stdio.h>
#include<string>
#include"thread.h"
using namespace std;

int ChatThread::sock;

ChatThread::ChatThread(char *ip, int port):ChatClient(ip, port) {

    sock = initClient();
}

void ChatThread::run(){

    char recvline[MAXLINE];
    char sendbuf[MAXLINE];
    
    string gname, uname;
    memset(recvline, 0, sizeof(recvline));
    memset(sendbuf, 0, sizeof(sendbuf));

    pthread_t pid;
    
    cout << "Please input the group Name: ";
    cin >> gname;
    cout << "Please input the user name: ";
    //scanf("%s", uname);
    cin >> uname;

    string sendMsg = gname + "%" + uname + " says:";
    //remove new line
    //strncpy(groupName, gname, strlen(gname));
    //strncpy(userName, uname, strlen(uname));
    //cin >> userName;
   // strcat(sendbuf, groupName);
    //strcat(sendbuf, "%");
    //strcat(sendbuf, userName);
    //strcat(sendbuf, "%"
    rio_writen(sock, (void*)sendMsg.c_str(), strlen(sendMsg.c_str()));
    //memset(sendbuf, 0, sizeof(sendbuf));
    pthread_create(&pid, NULL, readWork, NULL);
    //string sendMsg = groupName + "%" + userName + " says: ";
    while (true) {
        
        cout << "> ";
        //strcat(sendbuf, groupName);
        //strcat(sendbuf, "%");
        //strcat(sendbuf, userName);
        //strcat(sendbuf, " says: ");
        //scanf("%s", sendbuf + strlen(sendbuf));
        string msg;
        cin >> msg;
        //sendMsg += msg;
        rio_writen(sock, (void*)(sendMsg+msg).c_str(), strlen((sendMsg+ msg).c_str()));
        //memset(sendbuf, 0, sizeof(sendbuf));
        cout << endl;
    }

}

void *ChatThread::readWork(void *arg) {
    
    int n;
    int len1;
    int len2;
    string gname;
    string uname;
    char recvline[MAXLINE];
    memset(recvline, 0, sizeof(recvline));
    
    while(true) {

        n = read(sock, recvline, MAXLINE);
        recvline[n] = '\0';
        if (n < 0) {    
            cout << "Server disconnect\n";
            return NULL;
        }
        
        string recvString(recvline);

        len1 = recvString.find("%");
        //len2 = recvString.rfind("%");
        gname = recvString.substr(0, len1);
        //uname = recvString.substr(len1 + 1, len2 - len1);
        cout << "# " << recvline << endl;
        //puts(recvline + len1);
        memset(recvline, 0, sizeof(recvline));
    }

}


ssize_t ChatThread::rio_writen(int fd, void *userbuf, size_t n) {
    
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
