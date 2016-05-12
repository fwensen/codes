#include<iostream>
#include "haffuman.h"
using namespace std;

int main(int argc, char **argv) {
    
    if (argc != 3) {
        cout << "wrong input!" << endl;
        return 1;
    }
    string name = argv[2];
   
    //string name = "test.haf";
    if (!strcmp(argv[1], "c")) {
        HaffumanTree haff(name);
    } 

    if (!strcmp(argv[1], "x")) {
        cout << "decompress" << endl;
        HaffumanTree haff(name, 0);    
    }
    
}
