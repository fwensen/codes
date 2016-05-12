#include<iostream>
#include"GroupChat.h"
#include"socketWrap.h"
#include"rio_csapp.h"

using namespace std;

int GroupChat::servsock = -1;
map<string, Users*> GroupChat::group;
UserRegister* GroupChat::registerUser;


void GroupChat::initServer(int port) {

    servsock = ServerInit(port);
    registerUser = new UserRegister;
    registerUser->next = NULL;

}

string GroupChat::getGroupName(char *userBuf) {

    string buf(userBuf);
    buf = buf.substr(0, buf.find("%"));
    return buf;
}

void GroupChat::addUser(char *userBuf, int sock) {
    /*
    string buf(userBuf);
    buf = buf.substr(0, buf.find("%"));
    */
    string groupName = getGroupName(userBuf);

    map<string, Users*>::iterator iteUser;
    addUserRegister(sock, true);
    //寻找群组名，若没有找到，则添加群组名，并添加用户
    if ( (iteUser = group.find(groupName)) == group.end()) {
     
        //头节点，未用
        Users *mUsers = new Users;  mUsers->next = NULL;
        mUsers->sock = -1;

        Users *tmp ;
        tmp = new Users;
        tmp->sock = sock;
        tmp->next = mUsers->next;
        mUsers->next = tmp;

        group.insert(make_pair(groupName, mUsers));    //add
    } else {                       //找到群组名，在群组中添加用户

        cout << "found, and add a user\n";
        Users *tmp = new Users;
        tmp->sock = sock;
        tmp->next = iteUser->second->next;
        iteUser->second->next = tmp; 
    }
    cout << "added a user\n";
}

void GroupChat::removeUser(int sock) {
   /* 
    string buf(userBuf);
    buf = buf.substr(0, buf.find("%"));
    */
    map<string, Users*>::iterator iteUser = group.begin();
    removeUserRegister(sock);
    cout << "removed!\n";

    while (iteUser != group.end()) {

        Users *tmp = iteUser->second;
        while (tmp->next && tmp->next->sock != sock) {
            tmp = tmp->next;
        }

        if (tmp->next->sock == sock) {
            
            Users *T = tmp;
            tmp = T->next;
            delete T;
            break;
        }
        ++iteUser;
    }
}

void GroupChat::addUserRegister(int sock, bool b) {
    
    UserRegister *tmp = new UserRegister;
    tmp->sock = sock;  
    tmp->isRegister = b;

    tmp->next = registerUser->next;
    registerUser->next = tmp;
}

bool GroupChat::isUserRegister(int sock) {
    
    UserRegister *tmp = registerUser->next;
    while (tmp != NULL) {

        if (tmp->isRegister && tmp->sock == sock) 
            return true;
        tmp = tmp->next;
    }
    return false;
}

void GroupChat::removeUserRegister(int sock) {
    
    UserRegister *tmp = registerUser;
    while (tmp->next->sock != sock)
        tmp = tmp->next;

    if (tmp) {
        UserRegister *T = tmp->next;
        tmp = T->next;
        delete T;
    }
}


void GroupChat::sendGroupMsg(char *userBuf) {
    
    string groupName = getGroupName(userBuf);
    cout << "groupName: " << groupName << endl;
    string buf(userBuf);
    int len = buf.find("%");
    string msg = buf.substr(buf.find("%") + 1);
    map<string, Users*>::iterator ite;
    if ((ite = group.find(groupName)) == group.end()) {
        
    } else {

        Users* tmp = ite->second->next;
        while(tmp != NULL) {
            
            rio_writen(tmp->sock, userBuf+len+1, strlen(userBuf+len+1));
            cout << "Sended n: " << endl;
            tmp = tmp->next;
        }

    }

}
