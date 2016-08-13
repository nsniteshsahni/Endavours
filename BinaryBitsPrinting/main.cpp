#include <bits/stdc++.h>
#define max 100000
using namespace std;
int A[max]={0},i=0;
int N;
void Bin(int n)
{
    if(n<1)

    {
        for(int i=N-1;i>=0;i--)
            cout<<A[i];
        cout<<endl;
    }
else
    {
    A[n-1]=0;
    Bin(n-1);
    A[n-1]=1;
    Bin(n-1);
    }
}
int main()
{

    int n;
    cin>>n;
    N=n;
    Bin(n);
    return 0;
}
