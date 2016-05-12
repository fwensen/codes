#include<iostream>
#include "./OverlayClient.h"
#include<errno.h>
#include<cctype>


using namespace std;

int OverlayClient::sock;
string OverlayClient::groupId;
string OverlayClient::destId;
string OverlayClient::destIp;
int OverlayClient::destPort;

string OverlayClient::srcIp;
int OverlayClient::srcPort;


OverlayClient::OverlayClient(char *ip, int port) {

    client = new ClientSocket(ip, port);
    sock = client->getSocket();
    srcIp = client->getLocalIp();
    srcPort = client->getLocalPort();

    registerToServer();
    
    Thread thread(run, NULL);
    
    while (true) {

        communicationWithServer();
    }

}

OverlayClient::~OverlayClient(){

    delete client;
}


int OverlayClient::findChars(char *msg) {

    int len = strlen(msg);
    int start, i, count = 0;
    //char *tmp = msg;

    for (i = 10; i < 70; i++) {

        if (msg[i] == '$')
            ++count;

        if (count == 5) 
            break;
    }

    //strncpy(tmp, msg + i + 1, len - i - 1);
    return i + 1;
}

void OverlayClient::getDestinationInfo(char *msg) {

    cout << "recved msg: " << msg << endl;
    int len = strlen(msg);
    if (len < 8)
        return;
    string strMsg(msg);
    int startPos, endPos, i, count = 0;
    char desId[10]; memset(desId, 0, sizeof(desId));
    strncpy(desId, msg+10, 6);
    destId = desId;

    for (i = 8; i != 70; ++i) {
        if (msg[i] == '$') {
            ++count;
        
        if (count == 3) {
            startPos = i;
            continue;
        }

        if (count == 4) {
            endPos = i;
            break;
        }

        }
    }
   
    cout << "startPos: " << startPos << " endPos" << endPos << endl;
    if (i > 10)
        destIp = strMsg.substr(startPos + 1, endPos - startPos - 1);
    ++i;
    for (; i != 70; ++i) {
        if (msg[i] == '$') {
            startPos = endPos;
            endPos = i;
            break;
        }
    }
   
    cout << "startPos: " << startPos << " endPos" << endPos << endl;
    if (i > 10)
        destPort = atoi(strMsg.substr(startPos + 1, endPos-startPos).c_str());
    cout << "> " << destIp << " " << destPort << ": ";
}


void *OverlayClient::run(void *arg) {
   
    char recvLine[MSG_LEN]; 
    char recvCmd[10];
    int n;
    string recvString;

    while (true) {

        memset(recvLine, 0, sizeof(recvLine));
        memset(recvCmd, 0, sizeof(recvCmd));

        recvMsg(recvLine);

        if (recvLine)
            strncpy(recvCmd, recvLine, 4);
        else
            continue;

        if (strncmp(recvCmd, "MSGE", 4) == 0) {

            getDestinationInfo(recvLine);
            cout << recvLine + findChars(recvLine) << endl;
        } else if (strncmp(recvCmd, "SUCC", 4) == 0) {
          
            cout << "Destination exist!" << endl;
        } else if (strncmp(recvCmd, "FAIL", 4) == 0) {
            
            cout << "Destination not exist!" << endl;
            initWork();
        }
    }

    cout << "out of thread!" << endl;
}

void OverlayClient::communicationWithServer() {
    
    char sendLine[MSG_LEN];   memset(sendLine, 0, sizeof(sendLine));
    char constMsg[100];       memset(constMsg, 0, sizeof(constMsg));

    string stringMsg;
    while (true) {
        
        cout << endl <<  "> ";
        snprintf(constMsg, sizeof(constMsg), "%s%s%s$%s$%d$%s$%d$", groupId.c_str(), "MSGE", destId.c_str(), destIp.c_str(), destPort, srcIp.c_str(), srcPort);
        getline(cin, stringMsg);
        if (stringMsg == "CONN") {
            initWork();
            continue;
        }

        stringMsg = constMsg + stringMsg;
        sendMsg((char*)stringMsg.c_str(), strlen(stringMsg.c_str()));
    }

}

void OverlayClient::initWork() {
    
    char sendLine[MSG_LEN];  memset(sendLine, 0, sizeof(sendLine));
    char recvLine[MSG_LEN];  memset(recvLine, 0, sizeof(recvLine));
    char recvCmd[10];        memset(recvCmd, 0, sizeof(recvCmd));

    cout << "Input destination network ID: ";
    cin >> destId;
    cout << endl << "Input destination IP: ";
    cin >> destIp;
    cout << endl << "Input destination Port: ";
    cin >> destPort;
    cout << endl << "Request destination..." << endl;

    snprintf(sendLine, sizeof(sendLine), "%s%s%s$%s$%d$\0", groupId.c_str(), "CONN", destId.c_str(), destIp.c_str(), destPort);
    sendMsg(sendLine, strlen(sendLine));
   // recvMsg(recvLine);
   // strncpy(recvCmd, recvLine, 6);
   // if (strncmp(recvCmd, "SUCC", 4) == 0) {
    //    cout << "Destination exist!" << endl;
   // } else {

     //   cout << "Destination not exist!" << endl;
     //   exit(-1);
    //}
}

void OverlayClient::registerToServer(){
    
    char sendLine[MSG_LEN];    memset(sendLine, 0 , sizeof(sendLine));
    char recvLine[MSG_LEN];    memset(recvLine, 0, sizeof(recvLine));
    char recvCmd[CHARS_LEN];   memset(recvCmd, 0 , sizeof(recvCmd));
    int n;

    cout << "Input the network id: ";
    cin >> groupId;
    cout << "Register to server......" << endl;
    snprintf(sendLine, sizeof(sendLine), "%s%s$%s$%d$\0", groupId.c_str(), "REGI", srcIp.c_str(), srcPort);
    sendMsg(sendLine, strlen(sendLine));
    recvMsg(recvLine);
    strncpy(recvCmd, recvLine, 4);
    if (strncmp(recvCmd, "RGOK", 4) == 0) {
        cout << "Register success!" << endl;
    } else {
        cout << "Register failed!" << endl;
        exit(-1);
    }
}

// 
void OverlayClient::sendMsg(char *msg, int n) {
    
    int count = 0;
    int left = n;
    int pos = 0;
    cout << "send msg: " << msg << endl;
    while (left > 0) {
            
        count = send(sock, msg + pos, left, 0);
        if (count == -1) {

            cout << "Send error : " <<  strerror(errno) << endl;
            return;
        }
        left -= count;
        pos += count;
    }
}

void OverlayClient::recvMsg(char *buf) {
    
    int n;
    n = recv(sock, buf, MSG_LEN, 0);
    if (n == -1) {
        cout << "Recv error!" << endl;
        exit(-1);
    }
    buf[n] = '\0';
}
