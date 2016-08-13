#include <iostream>
#define t 100000
using namespace std;
int main()
{
    long long int A[t],cases,k;
    cin>>cases;
    for(long long int j=0;j<cases;j++)
        cin>>A[j];
    long long int count[t];

    for(long long int r=0;r<cases;r++)// Keep dividing n by powers of 5 and update count
   {

    for ( long long int i=5; A[r]/i>=1; i *= 5)

          {
              count[r] += A[r]/i;

          }
  }
for(k=0;k<cases;k++)
cout<<count[k]<<endl;
return 0;
}

