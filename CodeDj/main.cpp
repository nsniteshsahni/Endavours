#include <bits/stdc++.h>
using namespace std;
int main()
{
	int n,t,sum=0;
	n=5;
	int a[]={2,3,9,8,4};
	sort(a,a+n);
	t=a[n-1]+a[n-2];
	sum=t;
	for(int i=n-3;i>=0;i--)
	{
		t=t+a[i];
		sum=sum+t;
	}
cout<<sum;
}
