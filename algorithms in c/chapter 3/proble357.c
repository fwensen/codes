//3.57 ��Ŀ��ʵ�ֻ���ʶ�𣬲��Һ������еĿո�
#include<stdlib.h>
#include<stdio.h>
int judge(char *str)
{
    int len = strlen(str);
    int i,j,k=0,m=0;
    // m����k�ж����һ���֣���k����ÿ���жϺ�벿����
    //���пո�ʱ��ӵ�һ����

    for(i=0;i<len/2;i++)
    {
        if(str[i] == ' ')    //�ж�ǰ�沿���Ƿ���пո�
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
