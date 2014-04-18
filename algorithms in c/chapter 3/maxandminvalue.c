#include<math.h>
#include<stdio.h>
#include<stdlib.h>
typedef int Number;

Number randNum(int r){

    return rand()%r;
}
int main(){

   int i,N,r;
   float m1 = 0.0,m2=0.0;
   Number x;
   printf("please input N: ");
   scanf("%d",&N);
   printf("\nplease input r: ");
   scanf("%d",&r);
   for(i=0;i<N;i++)
    {

        x = randNum(r);
        m1 =+ ((float)x)/N;
        m2 =+ ((float) x*x)/N;
    }
        printf("Average: %lf\n", m1);
        printf("Deviation: %lf\n",sqrt(m2-m1*m1));

        return 0 ;

}











