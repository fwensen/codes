#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
int main()
{
    printf("size of int: %d\n", sizeof(int));
    printf("size of short: %d\n", sizeof(short));
    printf("size of long: %d\n", sizeof(long));
    printf("size of unsigned int: %d\n", sizeof(unsigned int));
    printf("size of unsigned short: %d\n", sizeof(unsigned short));
    printf("size of char: %d\n", sizeof(char));
    //int a = 0;
    //while(++a>0)
      //  printf("%d  ",a);
    printf("\n");

    //printf("pow = : %d \n", pow(2,16));
    srand((int)time(0));
    printf("rand: %d \n", rand());
    return 0;
}
