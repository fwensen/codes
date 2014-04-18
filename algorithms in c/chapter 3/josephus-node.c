
#include<stdlib.h>

typedef struct node* link;
struct node
{
    int item;
    link next;
};

int Item(link x);
link Next(link x);
link deleteNext(link x);
void insertNext(link x, link t);
void freeNode(link x);
link newNode(int i);
void initNodes(int N);

link freelist;
void initNodes(int N)
{
        int i;
        freelist = malloc((N+1)*(sizeof *freelist));
        for(i=0; i<N; i++)
            freelist[i].next = &freelist[i+1];
        freelist[N].next = NULL;
}

link newNode(int i)
{
    link x = deleteNext(freelist);
    x->item = i; x->next = x;
    return x;
}

void freeNode(link x)
{
    insertNext(freelist, x);
}

void insertNext(link x, link t)
{
    t->next = x->next;
    x->next = t;
}

link deleteNext(link x)
{
    link t = x->next; x->next = t->next;
    return t;
}

link Next(link x)
{
    return x->next;
}

int Item(link x)
{
    return x->item;
}


int main()
{
    int i, N = 100 ,M = 5;
    link t, x;
    initNodes(N);
    for(i=2,x=newNode(1); i<=N; i++)
    {
        t=newNode(i); insertNext(x,t);
        x=t;
    }
    while(x != Next(x))
    {
        for(i=1; i<N;i++)
            x=Next(x);
        freeNode(deleteNext(x));
    }

    printf("%d\n", Item(x));

    return 0;

}




















































