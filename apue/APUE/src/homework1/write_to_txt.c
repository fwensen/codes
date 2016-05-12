#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#define N 10

int main(){

    int  i;
    FILE *pf;
    char buf[10];
    pf = open("test.txt", O_RDWR);
    if (pf == NULL) {
        printf("open file error!\n");
        exit(0);
    }
    memset(buf, 0, sizeof(buf));
    for (i = 0; i < N; i++)  {
        sprintf(buf, "%d", i);
        write(pf, &buf, sizeof(buf));
        memset(buf, 0, sizeof(buf));
    }
    close(pf);
}

