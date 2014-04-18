//题目3.34: 编写一个函数，把给定链表中最大数据项移到该表中最后项
// 函数为shiftMax2End(link x)
//题目3.37：实现链表的一段代码，使两个给定的链接x,t指向的节点互换位置
//函数实现为changePosition(link x, link t)
//函数中还没实现释放节点内存
#include<stdlib.h>
typedef struct node* link;
struct node
{
    int item;
    link next;
};

link addNodes(link x, int N)
{
    int i;
    link x1 = x;
    for(i=N; i>0; i--)
    {
        link t =( malloc(sizeof *t));
        x1->next = t;t->item = i; t->next = NULL;
        x1 = x1->next;
    }

    return x;
}

void shiftMax2End(link x)
{
    link x1 = x->next, x2 = x;
    //link x3;
    int item = x->item;
    while(x1->next != NULL)
    {
        if(x1->item > item)
        {
            item = x1->item;
            //link x3 = x1;
        }
        x1 = x1->next;
    }
    if(item != x1->item)
    {
        link t = malloc(sizeof *t);
        x1->next = t; t->next = NULL; t->item = item;
    }
    else
        return ;
    /*
    if(x2->item == item)
    {
        free(x2);
        return ;
    }
    */
    while(x2->next->item != item)
        x2 = x2->next;
    x2->next = x2->next->next;

}

void showNodes(link x)
{
    link t = x->next;
    while(t!=NULL)
    {
        printf("%d ",t->item);
        t = t->next;
    }
    printf("\n");
}

link initNode()
{

    link x = malloc(sizeof *x), t ;
    x->next = NULL;
    x->item = -1;                         //头节点不作有效节点，只作辅助节点
    return x;

}

void changePosition(link x, link t)
{
    int m = x->next->item;
    int n = t->next->item;
    link x1 = x->next->next, t1 = t->next->next;
    link a = malloc(sizeof *a);
    link b = malloc(sizeof *b);
    x->next = a; a->item = n ; a->next =  x1;
    t->next = b; b->item = m; b->next = t1;

}
int main()
{
    link x, y;
    int N = 50, M = 60;
    printf("Beforing change...\n\n");
    printf("Node 1:\n");
    showNodes(x = addNodes(initNode(), N));
    printf("Node 2:\n");
    showNodes( y = addNodes(initNode(), M));
    //shiftMax2End(x);
    //showNodes(x);
    printf("\nAfter change...\n\n");
    changePosition(x->next->next, y->next->next->next);
    printf("Node 1:\n");
    showNodes(x);
    printf("Node 2:\n");
    showNodes(y);
    return 0;
}
