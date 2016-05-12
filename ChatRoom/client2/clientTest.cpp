#include<iostream>
#include"thread.h">
using namespace std;

int main(int argc, char* argv[]) {

    ChatThread chat(argv[1], atoi(argv[2]));
    chat.run();
}
