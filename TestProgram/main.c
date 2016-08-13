#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i=-3,j=2,k=0,m;
    m=++i && ++k || ++j;
    printf("%d",j);
    return 0;
}
