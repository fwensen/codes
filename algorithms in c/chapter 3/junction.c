// 3.26 题目： 给定两个指向不同循环链表的指针x和t， 把x的节点插入t的的下一节点
// junction(link x, link t)实现
// 3.27 题目：将x后的节点移到链表t的下一节点后面的位置上 anotherJunction(link x, link t)函数实现
#include<stdlib.h>
typedef struct node* link;
struct node
{
    int item;
    link next;
};

link junction(link x, link t)
{
    link x1 = x, t1;
    while(x1->next != x)
        x1 = x1->next;
    x1->next = x->next;                 //

    t1 = t->next;
    t->next = x; x->next = t1;
    return x;
}

link anotherJunction(link x, link t)
{
    link x1, t1;
    t1 = t->next;
    t->next = x->next;
    x->next = t1;
    return x;
}

link initNode(int N)
{
    int i;
    link t = malloc(sizeof *t), x = t;
    t->item = 1; t->next = t;
    for(i=2; i<N; i++)
    {
        x = (x->next = malloc(sizeof *x));
        x->item = i; x->next = t;
    }
    return t;
}
void test(link y)
{
    link y1 = y;
    printf("%d ",y->item);
    while(y1->next != y)
    {
        y1 = y1->next;
        printf("%d ",y1->item);
    }
}

int main()
{
    int M = 10, N = 15, i;
    link x, t, y;
    x = initNode(M);
    printf("nodes x: ");
    test(x);
    printf("\nnodes t: ");
    t = initNode(N);
    test(t);
    printf("\n");
    printf("x->item = %d\n",x->item);
    printf("t->item = %d\nAfter: ", t->item);
    y = anotherJunction(x, t);
    test(y);
    printf("\n");
    return 0;
}
