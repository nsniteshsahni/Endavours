#include <iostream>
#define max 10000
using namespace std;

int main()
{
   long int A[max],i,j;
    for(i=0;i<max;i++)
    {
        cin>>A[i];
    if(A[i]==42)
        break;
    }
    for(j=0;j<i;j++)
        cout<<"\n"<<A[j]<<endl;
    return 0;
}
