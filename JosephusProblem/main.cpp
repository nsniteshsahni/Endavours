#include <bits/stdc++.h>
using namespace std;
int Josephus(int n,int k)
{
if(n==1)
return 1;
else
{
return (Josephus(n-1,k)+k-1)%n+1;
}
}

int main() {
	// your code goes here
	int n,k;
	cin>>n>>k;
	cout<<Josephus(n,k);
	return 0;
}
