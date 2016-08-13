#include <iostream>
#include <conio.h>
#include <algorithm>
using namespace std;

int main()
{
    int A[100],B[100]={0},i,j,k,n;
    cin>>n;
    for(i=0;i<n;i++)
    cin>>A[i];
    for(i=0;i<n;i++)
        {
            for(j=0;j<n;j++)
            {
            if(A[i]<A[j])
            {

                B[i]=B[i]+A[j];
            }
            }
        B[i]=B[i]+A[i];
        }
    for(k=0;k<n;k++)
        cout<<B[k]<<" ";
    return 0;
}
