
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
    long long int t,a,b1=0,b2=0,b3=0,b4=0;
    long long int sum;
    cin>>a;
    sum=0;
    while(a>0)
    {
      if(a>=10)
      {
         b1=a/10;
         a=a%10;
      }

      else if(a>=7)
      {
         b2=a/7;
         a=a%7;
      }
      else if(a>=5)
      {
         b3=a/5;
         a=a%5;
      }
      else
      {
         b4=a;
         a=0;
      }
    }
    sum=b1+b2+b3+b4;
    cout<<sum<<endl;
    sum=0;
    }
}
