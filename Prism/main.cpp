#include <iostream>
#include <stdio.h>
using namespace std;

int main()
{
    int t;
    double a,b,c,x,y,z;
    double ar;
    cin>>t;
    for(int i=1;i<=t;i++)
    {
        cin>>x>>y>>z;
        a=(x+z-y)/2;
        b=(x+y-z)/2;
        c=(y+z-x)/2;
        ar=2*(a*b+b*c+a*c);
        printf("%.2f\n",ar);
    }
    return 0;
}
