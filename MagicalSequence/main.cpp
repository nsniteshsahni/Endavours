#include <bits/stdc++.h>
using namespace std;
int main()
{
	long long int n,c,i,j,sum=0;
	cin>>n>>c;
    vector<int> v(n);
    vector <int> x(n);
    for(i=0;i<n;i++)
    {
    	cin>>v[i];
    }
    for (i =0,j=n-1;abs(i-j)>=1; ++i)
    {
    	x[i]=v[i]-c;
    	x[i]=v[j]+c;
    }
    for(i=0;i<n;i++)
    {
    	for(j=i+1;j<n;j++)
    	{
    		sum+=abs(x[i]-x[j]);
    	}
    }
    cout<<sum;

}
