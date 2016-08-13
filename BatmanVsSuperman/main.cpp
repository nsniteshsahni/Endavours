#include <bits/stdc++.h>
#include <cmath>
using namespace std;

int main()
{
   // cout << "Hello world!" << endl;
    int n,k,count=0,a[10][10]={0},sum=0;
    cin>>n>>k;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
         cin>>a[i][j];
        }
    }
    for(int i=0;i<=n-k;i++)
    {
        for(int j=0;j<=n-k;j++)
        {
           for(int l=i;l<k+i;l++)
           {
               for(int p=j;p<k+j;p++)
               {
                   sum=sum+a[l][p];
               }
           }

        if((cbrt(sum)-ceil(cbrt(sum)))==0)
        {
            count++;
        }
        sum=0;
        }
    }
    cout<<count<<endl;
    return 0;
}
