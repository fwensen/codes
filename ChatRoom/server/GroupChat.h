#ifndef _GROUPCHAT_H
#define _GROUPCHAT_H
#include"socketWrap.h"
#include<string>
#include<map>

const static int G_MAX_USERS = 100;
//群组
typedef struct _User Users;

struct _User {

    int sock;
    Users* next;
};

//注册
typedef struct _UserRegister UserRegister;
struct _UserRegister {
    
    int sock;
    bool isRegister;
    UserRegister* next;
};

class GroupChat {

public:
    GroupChat() {};

protected:

    void initServer(int);
    static int servsock;
    static std::string getGroupName(char *);
    static void addUser(char *, int);
    static void removeUser(int);
    static std::map<std::string, Users*> group;
    static UserRegister* registerUser;

    static void addUserRegister(int, bool);
    static bool isUserRegister(int);
    static void removeUserRegister(int);

    static void sendGroupMsg(char *);
};

#endif
