#include<stdlib.h>

typedef struct node* link;
struct node
{
    int item;
    link next;

};

link initNodes()
{
    int i, N = 30;
    link x = malloc(sizeof *x), t=x;
    x->next = NULL; x->item = 1;
    for(i=2; i<N; i++)
    {
        t->next =  malloc(sizeof *x);
		t = t->next;
        t->next = NULL; t->item = i;
    }
    return x;
}

int main()
{
    int i = 1;
    link x = initNodes();
    link t, y = x;
    while(x!=NULL && i++)
    {
		x = x->next;
        if((i+1)%5 == 0)
        {
			t = x->next;
            free(t);
			x->next = x->next->next;
        }
    }

    return 0;





}








