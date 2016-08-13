#include<bits/stdc++.h>
using namespace std;
vector <long long int> ans;
void convert10tob(long long int N, int b)
{
     if (N == 0)
        return;
    long long int x = N % b;
     N /= b;
     if (x < 0)
        N += 1;
     convert10tob(N, b);
    ans.push_back(x < 0 ? x + (b * -1) : x);
     return;

}

int main()
{
    long long int n,q,i,prod=1,x,j,count=0;
    cin>>n;
    for(i=0;i<n;i++)
        {
          cin>>x;
          prod=prod*x;
        }

    cin>>q;
    while(q--)
    {
        int b;
        cin>>b;
        convert10tob(prod,b);
        for(j=ans.size()-1;j>=0;j--)
        {
        if(ans[j]==0)
        count++;
        else
        break;
        }
        cout<<count<<endl;
        count=0;
    }

    return 0;
}
