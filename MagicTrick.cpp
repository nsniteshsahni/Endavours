/*
MagicTrick.cpp by
@nsniteshsahni
Time Complexity: O(n^2)
*/
#include <bits/stdc++.h>
using namespace std;
int main()
{
	int t;
	cin>>t;
	while(t--)
	{
		int a,b,arr1[4][4],arr2[4][4],i,j,x,count=0; //arr1 is for 1st arrangment and arr2 is for second arrangment
		vector <int> temp1(4),temp2(4); //to store rows given by user
        cin>>a;
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				cin>>arr1[i][j];
		cin>>b;
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				cin>>arr2[i][j];
        
	    for(i=0;i<4;i++)
	    	temp1[i]=arr1[a-1][i];
	    for(j=0;j<4;j++)
	    	temp2[j]=arr2[b-1][j];
	    for(i=0;i<4;i++)
          {
	    	for(j=0;j<4;j++)
	    	{
	    		if(temp1[i]==temp2[j])
	    		{	count++;
	    		if(count==1)
	    			x=i;
	    		}
	    	}
	      }
	if(count==0)
		cout<<"Volunteer cheated!"<<endl;
	if(count==1)
		cout<<temp1[x]<<endl;
	if(count>1)
		cout<<"Bad magician!"<<endl;
	}
}