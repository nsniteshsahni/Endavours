#include <iostream>
#include <algorithm>
#define max 100000
using namespace std;

int main()
{
    long long int n,a[max],i,j,b[max];
    cin>>n;
    for(i=0;i<n;i++)
        cin>>a[i];
    for(j=0;j<max;j++)
        b[j]=count(a,a+n,j);
    for(i=0;i<n;i++)
    {
        if(b[i]!=0)
        cout<<i<<" "<<b[i]<<endl;
    }
    return 0;
}
