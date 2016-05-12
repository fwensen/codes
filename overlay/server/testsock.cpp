#include<iostream>
#include"./ServerSocket.h"
using namespace std;

int main(){

    ServerSocket *server = new ServerSocket(7777);
    cout << server->getSockFd() << endl;
}
