#include<stdlib.h>
//#define N 150
//#default M 6
typedef struct node* link;
struct node
{
    int item;
    link next;
};

int main()
{
    int i, N = 5,M = 4 ;
    link t = malloc(sizeof *t), x = t;
    t->item = 1;
    t->next = t;
    for(i=2; i<N; i++)
    {
        x = (x->next = malloc(sizeof *x));
        x->item = i; x->next = t;
    }

    while(x != x->next)
    {

        for(i=1; i<M; i++)
            x = x->next;
        x->next = x->next->next;
        N--;

    }
    printf("%d\n", x->item);
}
