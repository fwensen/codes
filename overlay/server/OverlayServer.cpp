#include<iostream>
#include"./OverlayServer.h"
#include<fcntl.h>
#include"../thread.h"
#include "Server.h"
#include<errno.h>

using namespace std;

char *OverLayServer::groupId;
list<CLIENTS> *OverLayServer::groupClients;
list<PEER_SERVER> *OverLayServer::peerServer;

OverLayServer::OverLayServer(int port, char *groupid) {

    ServerSocket *server = new ServerSocket(port);

    groupClients = new list<CLIENTS>;
    peerServer = new list<PEER_SERVER>;
    groupId = new char[CHARLEN];

    memset(groupId, 0 , sizeof(groupId));
    strncpy(groupId, groupid, 6);
    
    Thread *connThread = new Thread(connectOtherServer, NULL);
    //connectOtherServer();

    while (true) {

        int recvFd = server->ServerAccept();

        cout << "recvfd: " << recvFd << endl;
        if (recvFd != -1)
            Thread *recvThread = new Thread(recvMsg, (void*)&recvFd);
    }
    delete server;
}

OverLayServer::~OverLayServer(){
    
    /*      */
    delete peerServer;
    delete groupClients;
}

void *OverLayServer::connectOtherServer(void *arg) {
    
    string dsIp, dsId, cmd;
    int dsPort;
    while (true) {
    
        cout << "q: quit, conn: conn another server" << endl;
        cout << endl << ">";
        cin >> cmd;
        if (cmd == "q")
            break;
        else {
        
            cout << endl << "destination ID: ";
            cin >> dsId;
            cout << endl << "destination IP: ";
            cin >> dsIp;
            cout << endl << "destination Port:";
            cin >> dsPort;
            
            ClientSocket *client = new ClientSocket(const_cast<char*>(dsIp.c_str()), dsPort);
            
            //struct sockaddr_in cliaddr, servaddr;
            //bzero(&cliaddr, sizeof(cliaddr));
            //bzero(&servaddr, sizeof(servaddr));
            //int cli_sock = Sock_in_tcp4();


            int sock = client->getSocket();

            PEER_SERVER peer = new struct PEER_SERVER_;
            strncpy(peer->id, dsId.c_str(), 6);
            peer->sock = sock;

            peerServer->push_back(peer);
            //delete client;
        }
    }
    return NULL;
}

/*
SOCKINFO OverLayServer::initLink(char *ip, int port, int sockfd) {
    
    SOCKINFO newSock = new struct SOCKINFO_;
    if (newSock == NULL) {

        cout << "new error!" << endl;
        exit(0);
    }
    newSock->addr = new char[CHARLEN];
    newSock->groupId = new char[CHARLEN];
    newSock->name = new char[CHARLEN];
    newSock->dstId = new char[CHARLEN];
    newSock->msgSend = new char[MSGLEN];
    newSock->msgRecv= new char[MSGLEN];

    memcpy(newSock->addr, ip, sizeof(ip));
    newSock->port = port;
    newSock->sockfd = sockfd;
    //newSock->next = NULL;

    return newSock;
}
*/
void *OverLayServer::recvMsg(void *arg) {
    
    int sock, n;
    sock = *(int*)arg;
    char recvBuf[1500];  //memset(recvBuf, 0, sizeof(recvBuf));

    while (true) {

        memset(recvBuf, 0, sizeof(recvBuf));
        n = recv(sock, recvBuf, sizeof(recvBuf), 0);
        if (n <= 0 || errno == EINTR) {
            cout << "Disconnect by peer point!" << endl;
            return NULL;
        }  

        recvBuf[n] = '\0';
        handleMsg(recvBuf, sock);
        //cout << "recvMsg: " << recvBuf << endl;
    }
    return NULL;
}

void OverLayServer::sendMsg(char *arg, int sock) {
    
    int n;
    cout << "sendMsg: " << arg << endl;
    n = write(sock, arg, strlen(arg));
    if (n == -1) {
        cout << "send error!" << endl;
        return;
    }

    cout << "sended: " << n << endl;

}

//just for localhost test
CLIENTS OverLayServer::findClient(int port) {

    list<CLIENTS>::iterator ite;
    for (ite = groupClients->begin(); ite != groupClients->end(); ++ite) {
        if ((*ite)->srcPort == port)
            return *ite;
    }
    return NULL;
}

//just for localhost test
PEER_SERVER OverLayServer::findPeerServer(int sock, char *dstId) {
    
    list<PEER_SERVER>::iterator beg = peerServer->begin();
    if (dstId == NULL) {
        for (; beg != peerServer->end(); ++beg) {
            if ((*beg)->sock == sock)
                return *beg;
        }
    } else {
        
        for (; beg!=peerServer->end(); ++beg) {

            if (strcmp((*beg)->id, dstId) == 0)
                return *beg;
        }
    }
    return NULL;
}

char *OverLayServer::findSpecificString(char *msg, int mTh) {

    char *result = (char*)malloc(CHARLEN * sizeof(char)); 
    memset(result, 0, sizeof(result));

    int i, count = 0, start, end;
    for (i = 8; i < 70; ++i) {
    
        if (msg[i] == '$')
            ++count;
        if (count == mTh)
            break;
        }
    start = i;
    ++i;
    for (; i < 70; ++i)
        if (msg[i] == '$')
            break;
    end = i;
    start += 1;
    end += 1;
    for (i = start; i < end - 1; ++i) {

        result[i-start] = *(msg + i);
        //cout << msg[i] << endl;
    }
    //strncpy(result, msg + start + 1, end - start);
    cout << "result: " << result << "start: " << start <<"end: " << end << " and : " << msg << endl;
    return result;
}



void OverLayServer::handleMsgFromClient(char* msg, int sock) {

    //MSG_TYPE msgRcv = (MSG_TYPE)msg;
    //local group
    char destId[10]; memset(destId, 0, sizeof(destId));
    char echoMsg[MSGLEN];   memset(echoMsg, 0, sizeof(echoMsg));

    strncpy(destId, msg+10, 6);
    

    char *destPort;

    cout << "destID: " << destId << endl;
    if (strncmp(destId, groupId, 6) == 0) {

        destPort = findSpecificString(msg, 2);
        CLIENTS cli = findClient(atoi(destPort));
        cout << "destPort: " << atoi(destPort) << " int list: " << " sock: " << cli->sock << cli->srcPort << endl;
        if (cli)
        {
            strncpy(echoMsg, msg+6, strlen(msg) - 6);
            sendMsg(echoMsg, cli->sock);
        }
    //other group
    } else {

        PEER_SERVER peer = findPeerServer(0, destId);
        if (peer)
            sendMsg(msg + 6, peer->sock);
    }   

    //free(destPort);
}

void OverLayServer::handleRegister(char *msg, int sock) {
   
    char echoMsg[MSGLEN];

    CLIENTS client = new struct GROUP_CLIENTS_;
    memset(client->srcIp, 0, sizeof(client->srcIp));

    //struct sockaddr_in cli;
    //socklen_t len;
    //memset(&cli, 0, sizeof(cli));

    //if (getpeername(sock, (struct sockaddr*)&cli, &len) != 0) {
    //    cout << "get peer name error!" << endl;
    //    return;
    //}

    char *srcIp;
    char *srcPort;
    srcIp = findSpecificString(msg, 1);
    srcPort = findSpecificString(msg, 2);

    //srcPort = atoi(srcPort);
    strncpy(client->srcIp, srcIp, strlen(srcIp));
    client->srcPort = atoi(srcPort);

    cout << "Register a client: " << srcIp << "$" << srcPort << endl;
    client->sock = sock;

    groupClients->push_back(client);

    snprintf(echoMsg, sizeof(echoMsg), "%s\0","RGOK");
    sendMsg(echoMsg, sock);

    //free(srcIp);
    //free(srcPort);

}

void OverLayServer::handleConnFromPeerServer(char *msg, int sock) {

    //MSG_TYPE msgRcv = (MSG_TYPE)msg;

    PEER_SERVER peer = new struct PEER_SERVER_;
    memcpy(peer->id, msg, 6);
    peer->sock = sock;
    peerServer->push_back(peer);
    
}

void OverLayServer::handleSucc(char *msg, int sock) {
    
    //MSG_TYPE msgRcv = (MSG_TYPE)msg;
    char echoMsg[MSGLEN];  memset(echoMsg, 0, sizeof(echoMsg));
    //char *dPort = (char*)malloc(10*sizeof(char)); memset(dPort, 0, sizeof(dPort));
    char *dPort;
    snprintf(echoMsg, sizeof(echoMsg), "%s\0", "SUCC");
    dPort = findSpecificString(msg, 2);

    CLIENTS cli = findClient(atoi(dPort));
    if (!cli)
        sendMsg(echoMsg, cli->sock);

    //free(dPort);
}

void OverLayServer::handleFailed(char *msg, int sock) {

    //MSG_TYPE msgRcv = (MSG_TYPE)msg;
    char echoMsg[MSGLEN]; memset(echoMsg, 0, sizeof(echoMsg));
    //char *dPort = (char*)malloc(10*sizeof(char)); memset(dPort, 0, sizeof(dPort));
    char *dPort;
    snprintf(echoMsg, sizeof(echoMsg), "%s\0", "FAIL");
    dPort = findSpecificString(msg, 2);

    CLIENTS cli = findClient(atoi(dPort));
    if (!cli)
        sendMsg(echoMsg, cli->sock);

    //free(dPort);
}

void OverLayServer::handleMsgFromPeerServer(char *msg, int sock) {
    
    //MSG_TYPE msgRcv = (MSG_TYPE)msg;
    //char *dPort = (char*)malloc(10*sizeof(char)); memset(dPort, 0, sizeof(dPort));
    char *dPort;
    dPort = findSpecificString(msg, 2);
    
    CLIENTS cli = findClient(atoi(dPort));
    if (cli)
        sendMsg(msg + 6, cli->sock);
    free(dPort);
}

void OverLayServer::handleRequest(char *msg, int sock) {
    
    char echoMsg[MSGLEN]; memset(echoMsg, 0, sizeof(echoMsg));  
    //snprintf(echoMsg->id, sizeof(echoMsg->id), "%s", groupId);
    //char *sPort = (char*)malloc(sizeof(char)*10);  memset(sPort, 0, sizeof(sPort));
    //char *sIp = (char*)malloc(32*sizeof(char));    memset(sIp, 0, sizeof(sIp));
    //char *dPort = (char*)malloc(10*sizeof(char));  memset(dPort, 0, sizeof(sPort));
    //char *dIp = (char*)malloc(32*sizeof(char));    memset(dIp, 0, sizeof(dIp));
    char dId[10]; memset(dId, 0, sizeof(dId));
    char *sPort, *sIp, *dPort, *dIp;

    sPort = findSpecificString(msg, 4);
    sIp = findSpecificString(msg, 3);
    dPort = findSpecificString(msg,2);
    dIp = findSpecificString(msg, 1);
    strncpy(dId, msg, 6);

    strncpy(echoMsg, groupId, strlen(groupId));

    CLIENTS cli = findClient(atoi(dPort));
    if (cli || findClient(atoi(dPort)))
        snprintf(echoMsg + 6, sizeof(echoMsg), "%s", "SUCC");
    else
        snprintf(echoMsg + 6, sizeof(echoMsg), "%s", "FAIL");
    
    strncpy(echoMsg + 10, dId, strlen(dId));
    snprintf(echoMsg + 16, sizeof(echoMsg) - 16, "$%s$%s$%s$%s$\0", sIp, sPort, dIp, dPort);

    sendMsg((char*)echoMsg, sock);
    
    //delete echoMsg;
    //free(dPort);
    //free(dIp);
    //free(sIp);
    //free(sPort);
}



////********************////
void OverLayServer::handleConnToPeer(char *msg, int sock) {
    
    char destId[10];        memset(destId, 0, sizeof(destId));
    char sendLine[MSGLEN];  memset(sendLine, 0, sizeof(sendLine));
    //char *destIp = (char*)malloc(CHARLEN*sizeof(char));   memset(destIp, 0, sizeof(destIp));
    //char *destPort = (char*)malloc(10*sizeof(char)); memset(destPort, 0, sizeof(destPort));

    char *destIp;
    char *destPort;
//    string destIp;
//    string destPort;

    strncpy(destId, msg + 10, 6);
    destIp = findSpecificString(msg, 1);
    destPort = findSpecificString(msg, 2);
/*
    string sdMsg = groupId;
    sdMsg += "RQST";
    sdMsg += destId;
    sdMsg += "$";
    sdMsg += destIp;
    sdMsg += "$";
    sdMsg += destPort;
    sdMsg += "$";
    cout << "sdMsg: "<< sdMsg << endl;
  */
    snprintf(sendLine, sizeof(sendLine), "%s%s%s", groupId, "RQST", destId);
    snprintf(sendLine + 16, sizeof(sendLine)-16, "$%s$%s$", destIp, destPort);
    if (strncmp(destId, groupId, 6) != 0) {
        
        PEER_SERVER serv = findPeerServer(0, destId);
        if (serv) {
            sendMsg(sendLine, serv->sock);    
        }
    } else {
        
        CLIENTS cli = findClient(atoi(destPort));
        char response1[10];  memset(response1, 0, sizeof(response1));
        char response2[10];  memset(response2, 0, sizeof(response2));
        snprintf(response1, sizeof(response1), "%s", "SUCC");
        snprintf(response2, sizeof(response2), "%s", "FAIL");
        if (cli)
            sendMsg(response1, sock);
        else
            sendMsg(response2, sock);
    }

    //free(destIp);
    //free(destPort);
}

void OverLayServer::handleMsg(char *msg, int sock) {
   
    char idBuf[10];   memset(idBuf, 0, sizeof(idBuf));
    char cmd[CHARLEN]; memset(cmd, 0, sizeof(cmd));
    
    strncpy(idBuf, msg, 6);
    strncpy(cmd, msg+6, 4);

    cout << "\nrecvMsg: " << msg << endl;

    cout << "group: "  << idBuf << "\ncommand: " << cmd << endl;
    cout << "localgroup: " << groupId << endl;
    // in local group 
    //cmd = msgRcv->cmd;
    int n;
    if ( (n = strncmp(idBuf, groupId, 6)) == 0) {

       if (strncmp(cmd, "MSGE", 4) == 0) {

           handleMsgFromClient(msg, sock);
       } else if (strncmp(cmd, "CONN", 4) == 0) {

            handleConnToPeer(msg, sock);
       } else if (strncmp(cmd, "REGI", 4) == 0) {

           handleRegister(msg, sock);
       } 

    } else {

        if (strncmp(cmd, "RQST", 4) == 0) {
            
            handleRequest(msg, sock);
        } else if (strncmp(cmd, "MSGE", 4) == 0) {

            handleMsgFromPeerServer(msg, sock);
        } else if (strncmp(cmd, "SUCC", 4) == 0) {
            
            handleSucc(msg, sock);
        } else if (strncmp(cmd, "FAIL", 4) == 0) {
        
            handleFailed(msg, sock);
        } else if (strncmp(cmd, "CONN", 4) == 0) {
            handleConnFromPeerServer(msg, sock);        
        }
    }
}

int main() {
    
    OverLayServer *over = new OverLayServer(7777, "111111");    

}
