//3.24 ��Ŀ����дһ�����������нڵ����ĺ���������ָ��ѭ��������ĳ���ڵ��ָ��
#include<stdlib.h>
typedef struct node* link;
struct node
{
    int item;
    link next;
};

int getTheNum(link x)
{

    int num = 0;
    link x1 = x;
    link x2 = x;
    while(x2->next != x1)
    {
        ++num;
        x2 = x2->next;
    }
    ++num;
    return num;
}
int main()
{
    int N, num, i;
    printf("�����ж��ٸ��ڵ�: ");
    scanf("%d",&N);
    link t = malloc(sizeof *t), x = t;
    t->item = 1; t->next = t;
    //��ʼ��ѭ������
    for(i=2; i<=N; i++)
    {
        x = (x->next = malloc(sizeof *x));
        x->item = i; x->next = t;
    }
    num = getTheNum(t);
    printf("\nThe sum node is: %d\n", num);
    return 0;

}
