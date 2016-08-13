#include <iostream>
using namespace std;

int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        long long int l,r,count=0,i,j;
        cin>>l>>r;
        for(i=l;i<=r;i++)
        {
            if(i==0)
                count++;
            if((i^(i+1))==1)
                {
                    count++;
                }
        }
   cout <<count<< endl;
    }
    return 0;
}
