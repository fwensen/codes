#include"FileHandler.h"
#include<iostream>
#include<sys/time.h>

const int MAX_CHAR = 1000000;
using namespace std;

int main(){

  // 测试读写完整性.
  /*
    FileHandler file("test.txt"), file2("log.txt");
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    file.OpenFile(O_RDWR);
    file2.OpenFile(O_RDWR);
    for (int i = 0; i < 100; i++) {

        file.ReadFile(buf, 1024);
        file2.WriteFile(buf, 1024);
        cout << buf ;
        memset(buf, 0, sizeof(buf));
    }
*/

//测试读文件效率****************

    FileHandler *file = new FileHandler("test2.txt");
    file->OpenFile(O_RDONLY);
    char buf[2];  
    memset(buf, 0, sizeof(buf));
    struct timeval tv1, tv2, tv3, tv4;
    gettimeofday(&tv1, 0);
    for (int i = 0; i < MAX_CHAR; ++i)
        file->ReadFile(buf, 1);
    gettimeofday(&tv2, 0);
    long t = 1000000*(tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec);
    cout << "In ReadFile, The time is: " << t << endl;
    delete file;

    memset(buf, 0, sizeof(buf));
    int fd = open("test2.txt", O_RDONLY);
    gettimeofday(&tv3, 0);
    for (int i = 0; i < MAX_CHAR; ++i)
       read(fd, buf, 1);
    gettimeofday(&tv4, 0);
    t = 1000000*(tv4.tv_sec - tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec);
    cout << "In read, The time is: " << t << endl;
    close(fd);
    //****************************

/*
    //测试写文件效率
    FileHandler *file = new FileHandler("test3.txt");
    file->OpenFile(O_WRONLY|O_CREAT);
    char buf[2];  
    memset(buf, 0, sizeof(buf));
    struct timeval tv1, tv2, tv3, tv4;
    gettimeofday(&tv1, 0);
    for (int i = 0; i < MAX_CHAR; ++i)
        file->WriteFile("1", 1);
    gettimeofday(&tv2, 0);
    long t = 1000000*(tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec);
    cout << "In WriteFile, The time is: " << t << endl;
    delete file;

    memset(buf, 0, sizeof(buf));
    int fd = open("test4.txt", O_WRONLY | O_CREAT);
    gettimeofday(&tv3, 0);
    for (int i = 0; i < MAX_CHAR; ++i)
       write(fd, "2", 1);
    gettimeofday(&tv4, 0);
    t = 1000000*(tv4.tv_sec - tv3.tv_sec) + (tv4.tv_usec-tv3.tv_usec);
    cout << "In write, The time is: " << t << endl;
    close(fd);
    //**********************************************************
*/
}
