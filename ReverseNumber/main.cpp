#include <bits/stdc++.h>
using namespace std;
long long int reverse (long long int prod)
{
  long long int temp=0;
  while(prod>0)
        {
          if(prod%10==0)
            prod=prod/10;
          else
            break;
        }
      while(prod != 0)
        {
     long long int remain = prod%10;
      temp = temp*10 + remain;
      prod/=10;
      }

       return temp;
}
int main()
{
  int t;
  cin>>t;
  while(t--)
    {
      long long int n,k,i,count=1;
      cin>>n>>k;
      i=k;
      while(i<pow(10,n))
      {
        if(i%k==0&&reverse(i)%k==0)
          count++;
        i++;
      }
      cout<<count%1000000007<<endl;
    }
}
