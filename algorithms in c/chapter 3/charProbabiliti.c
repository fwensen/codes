
//3.56 ��Ŀ����ӡһ���ַ����������ַ����ַ����г��ֵĸ���  ps�����˷��˺ܶ��ڴ�
//�ǳ��Ĳ���Ч
#include<stdlib.h>
#include<stdio.h>
struct stri
{
    char charStr;
    int num;
    double proba;
};

printProba(char* str,struct stri stringS[])
{
    int len = strlen(str);
    int i,j;
    for(i=0;i<len;i++)
    {
        for(j=0;j<len;j++)
        {
            if(str[i] == stringS[j].charStr)
                stringS[j].num += 1;
        }
    }
    for(i=0;i<len;i++)
        stringS[i].proba = (double)stringS[i].num/(double)len;


}

int main()
{
    char *str = "qwertqwertqwerqwegfdsgdfggnfvrwweerrqqwerqwertewe";
    int N = strlen(str);
    struct stri stringS[N];
    int i,j,m;
    for(i=0;i<N;i++)
    {
        stringS[i].charStr = str[i];
        stringS[i].num = 0;
        stringS[i].proba = 0;
    }
    printProba(str,stringS);
    for(i=0;i<N;i++)
    {
        for(j=0;j<i;j++)
            if(stringS[j].charStr == stringS[i].charStr)
                break;
        if(j==i)
        {
            printf("%c, %f : ",stringS[i].charStr,stringS[i].proba);
            for(m=0;m<stringS[i].num;m++)
                printf("*");
            printf("\n");
         // printf("%c : %f \n",stringS[i].charStr, stringS[i].proba);
        }

    }



    return 0;
}

