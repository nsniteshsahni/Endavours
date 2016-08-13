#include <bits/stdc++.h>
using namespace std;
int main()
{
	int arr[][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
	int i,j,k, size=4, middle;
	for(int i=size-1, j=0;i>0;i--,j++)
	{
		for (k = j; k<i; ++k)
		{
			cout<<arr[j][k];
		}
		for (k=j; k <i ; ++k)
		{
			cout<<arr[k][i];
		}
		for (k = i; k>j; --k)
		{
			cout<<arr[i][k];
		}
		for (k = i; k>j; --k)
		{
			cout<<arr[k][j];
		}
	}
	middle=(size-1)/2;
	if(size%2==1)
		cout<<arr[middle][middle];
	return 0;
}
