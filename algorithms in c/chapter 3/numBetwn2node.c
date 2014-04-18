// 3.25 题目：给定指向循环链表的两个节点x,t， 确定这两个节点间的节点数
#include<stdlib.h>
typedef struct node* link;
struct node
{
    int item;
    link next;
};

getNumBetwn2Node(link x, link t)
{
    link x1 = x, t1 = t;
    int num = 0;
    while(x1->next != t1)
    {
        num++;
        x1 = x1->next;
    }

     return ++num;

}


int main()
{
    int i; int N = 100, num;
    link t = malloc(sizeof *t), x = t;
    t->item = 1; t->next = t;
    for(i=2; i<N; i++)
    {
        x = (x->next = malloc(sizeof *t));
        x->item = i; x->next = t;
    }

    num = getNumBetwn2Node(t->next, t->next->next->next->next);
    printf("The num between 2 nodes is: %d\n", num);
    return 0;
}












