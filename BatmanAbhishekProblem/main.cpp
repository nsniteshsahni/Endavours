#include <iostream>
using namespace std;
#define max 100000
int main()
{   int t;
    cin>>t;
    while(t--)
    {
    unsigned long long int a[max],m,n,k,i,j,count=0;
    cin>>m>>n>>k;
    for(i=0;i<m;i++)
        cin>>a[i];
        for(i=0;i<n;i++)
            {
                if(a[i]<k)
                    count++;
            }
    if(count==n)
    cout<<"YES SIR"<<endl;
    else
    cout<<"NO SIR"<<endl;
    }
    return 0;
}
