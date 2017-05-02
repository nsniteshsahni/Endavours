#include <bits/stdc++.h>
using namespace std;
bool dp[1000];
bool SubSetSum(vector <int> v,int k)
{
 dp[0]=1;
 for(int i=0;i<v.size();i++)
 {
 	for(int j=k;j>=v[i];j--)
 	{
 		dp[j] |= dp[j-v[i]];
 	}
 }
  return dp[k];
}
int main() {
	// your code goes here
	vector <int> a = {3,2,7,1,9};
	int sum=0;
	cout<<SubSetSum(a,sum);
	return 0;
}