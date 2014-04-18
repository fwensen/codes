#include<stdio.h>
#include<math.h>
//#include "point.h"
#include<stdlib.h>


typedef struct
{
    float x;
    float y;
}point;

//float distance(point, point);

//int onTheSameLine(point, point, point);

int onTheSameLine(point a, point b, point c)
{
      if((c.y == ( b.y + ((b.y-a.y)/(b.x - a.x))*(c.x-b.x))))
        return 1;
    else
        return 0;
}

int main()
{
    int onTheSameLine(point, point, point);
    point a,b,c;
    printf("input a,b,c:\n");
    scanf("%f %f %f %f %f %f",&a.x,&a.y,&b.x,&b.y,&c.x,&c.y);
    if(onTheSameLine(a,b,c))
        printf("They are on the same line\n");
    else
        printf("They are not on the same line\n");
    return 0;
}
