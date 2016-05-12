#include<iostream>
#include"./socketWrap.h"
#include"./thread.h"
using namespace std;

void *doit(void *arg) {

    cout << "in doit()!" << endl;
}


int main() {

    Thread *thread = new Thread(doit, NULL);
    delete thread;
}
