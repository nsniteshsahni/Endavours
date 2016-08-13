#include <iostream>
#include <math.h>
using namespace std;

int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        int num,n,sum;
        cin>>num>>n;
        while(n--)
        {
            sum=0;
            while(num>0)
            {
                int r=num%10;
                sum=sum+r*r;
                num=num/10;
            }
        num=sum;
        }
    cout<<sum<<endl;
    }
    return 0;
}
