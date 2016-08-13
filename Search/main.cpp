#include <iostream>
using namespace std;
#define max 1000000
int main()
{
    unsigned long long int n,x,q,a[max],i,j,temp,k;
    int count=0;
    cin>>n;
    cin>>q;
    for(i=0;i<n;i++)
    {
    cin>>a[i];
    }
    for(j=0;j<q;j++)
    {
    cin>>temp;
    for(k=0;k<n;k++)
    {
    if(temp==a[k])
    count++;
    }
    if(count>0)
    cout<<"YES/n";
    else
    cout<<"NO/n";
    }
    return 0;
}
