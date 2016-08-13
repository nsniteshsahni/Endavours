#include <bits/stdc++.h>
#define max 100000
using namespace std;
int F[max]; //Prefix Array
void Prefix(string P)
{
	int i=1,j=0;
	int m = P.length();
    while(i<m)
    {
    F[0]=0;
    if(P[i]==P[j])
    {
    	F[i]=j+1;
    	i++;
    	j++;
    }
    else if(j>0)
    {
    	j=F[j-1];
    }
    else
    {
    	F[i]=0;
    	i++;
    }
 }
}

int KMP_Matcher(string T,string P)
{
	int n=T.length();
	int i=0,j=0;
	Prefix(P);
	while(i<n)
	{
		if(T[i]==P[i])
		{
			if(j==P.length()-1)
			{
				return i-j;
			}
			else
			{
				i++;
				j++;
			}
		}
		else if(j>0)
		{
			j=F[j-1];
		}
		else
		{
			i++;
		}

    }
		return -1;
}

int main()
{
    string str,ptr;
    cout<<"Enter The String\n";
    cin>>str;
    cout<<"Enter The Pattern To Be Searched\n";
    cin>>ptr;
    cout<<"Match Found At "<<KMP_Matcher(str,ptr);
	return 0;
}
