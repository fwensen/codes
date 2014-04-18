//3.57 题目：实现回文识别，并且忽略其中的空格
#include<stdlib.h>
#include<stdio.h>
int judge(char *str)
{
    int len = strlen(str);
    int i,j,k=0,m=0;
    // m代表k中多减的一部分，而k代表每次判断后半部分是
    //否有空格时多加的一部分

    for(i=0;i<len/2;i++)
    {
        if(str[i] == ' ')    //判断前面部分是否带有空格
            {
                m++;
                continue;
            }
        j = len-i-1-k+m;
       while(str[j] == ' ')
           {
            j -= 1;
            k++;
           }
        if(str[i] != str[j])
            return 0;
    }
    return 1;
}

int main()
{
    char *str = "lhk jrql ndla  g   galdnlqrjkhl";
    int i = judge(str);
    if(i)
        printf("right!\n");
    else
        printf("wrong!\n");

    return 0;

}
