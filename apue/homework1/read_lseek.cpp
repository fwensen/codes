#include<fcntl.h>
#include<unistd.h>
#include<iostream>
#include<cstring>
using namespace std;

int main(){

    int fd = open("test.txt", O_RDONLY);
    if (fd == -1) {
        cout << "open error!\n";
        return 0;
    }

    char buf[36];
    memset(buf, 0 ,sizeof(buf));
    if (read(fd, buf, sizeof(buf)) == -1) {
        cout << "read error!\n";
        return 0;
    }
    cout << "All characters: ";
    cout << buf;
    memset(buf, 0, sizeof(buf));
    close(fd);

    cout << "Opening file with O_APPEND..." << endl;
    fd = open("test.txt", O_RDONLY | O_APPEND); //open file with O_APPEND
    if (fd == -1) {
        cout << "open file error!" << endl;
        return 0;
    }
    
    //read without seek
    if (read(fd, buf, 1) == -1) {
        cout << "read error!" << endl;
        return 0;
    }

    cout << "Read the first character without seek: ";
    cout << buf << endl;
    lseek(fd, 3, SEEK_SET);      //seek at 4 character
    if ( read(fd, buf, 1) == -1 ) {
        cout << "read error!" << endl;
        return 0;
    }
    cout << "After seek at 4th character: ";
    cout << buf << endl;

    lseek(fd, 6, SEEK_SET); // seek at 7th character
    if ( read(fd, buf, 1) == -1 ) {
        cout << "read error!" << endl;
        return 0;
    }
    cout << "After seek at 7th character: ";
    cout << buf << endl;
    close(fd);

}
