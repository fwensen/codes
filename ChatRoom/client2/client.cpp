#include<iostream>
#include"client.h"

using namespace std;

char *ChatClient::groupName;
char *ChatClient::userName;

ChatClient::ChatClient(char *Ip, int port) {

    groupName = new char[32];
    userName = new char[32];
    memset(ip, 0, sizeof(ip));
    strcpy(ip, Ip);
    port = port;
    memset(groupName, 0, sizeof(groupName));
    memset(userName, 0, sizeof(userName));
}

int ChatClient::initClient() {
    int sock = ClientInit(ip, port);
    return sock;
}
