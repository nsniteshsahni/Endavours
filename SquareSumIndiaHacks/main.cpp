#include <bits/stdc++.h>
using namespace std;
int main()
{
	long long int n,k,i,j,x,sum=0;
	cin>>n>>k;
	vector<int> v(n);
	for(i=0;i<n;i++)
		cin>>v[i];
	j=0;
	for(i=0;i<=n-k;i++)
	{

			for(x=0;x<=n;x++)
			{

			  sum=sum+(j+1)*(j+1)*v[i+j];
			  j++;
			}

       cout<<sum<<" ";
       sum=0;
    }
	return 0;
}
