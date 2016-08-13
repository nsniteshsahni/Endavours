#include <iostream>
#include <algorithm>
using namespace std;
#define max 100
int main()
{
    unsigned long long int s[max],r[max],n,i,j,streak[max]={0},sum=0;
    int t,c=0;
    cin>>t;
    while(t--)
    {
    	cin>>n;
    	for(i=0;i<n;i++)
    	cin>>s[i]>>r[i];
    	for(j=0;j<n;j++)
    	{
    		if(r[j]==1&&count(s,s+n,s[j])==1)
    	    streak[j]++;
    	}
    for(i=0;i<n;i++)
    {
     if(count(s,s+n,s[i])!=1&&r[i]==1)
        c++;
    sum=sum+streak[i];
    }
    if(c>0)
       cout<<sum+1<<endl;
    else
       cout<<sum<<endl;

    }
    return 0;
}
