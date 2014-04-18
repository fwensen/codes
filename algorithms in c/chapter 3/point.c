#include<math.h>
#include "point.h"

float distance(point a, point b)
{
    float dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx*dx - dy*dy);
}
int onTheSameLine(point a, point b, point c)
{

      if((c.y == ( b.y + ((b.y-a.y)/(b.x - a.x))*(c.x-b.x))))
        return 1;
    else
        return 0;

}
