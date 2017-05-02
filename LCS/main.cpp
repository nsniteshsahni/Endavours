#include <bits/stdc++.h>
using namespace std;
int LCS(string s1,string s2)
{
    const int m = s1.length(), n = s2.length();
    int temp[m+1][n+1];
    for(int i=0;i<=m;i++)
    {
        for(int j=0;j<=n;j++)
        {
            if(i==0||j==0)
                temp[i][j]=0;
            else if(s1[i-1]==s2[j-1])
                temp[i][j] = temp[i-1][j-1]+1;
            else
                temp[i][j] = max(temp[i-1][j],temp[i][j-1]);
        }
    }
 int len = temp[m][n];
 char str[len+1];
 str[len] = '\0';
 int i = m,j = n;
 while(i>0&&j>0)
 {
    if(s1[i-1]==s2[j-1])
     {
     str[len-1]=s1[i-1];
     len--;i--;j--;
     }
     else if(temp[i-1][j]>temp[i][j-1])
        i--;
     else
        j--;
 }
 cout<<str<<endl;
 return temp[m][n];
}
int main()
{
    string s1="ABCADX",s2="ACGDHX";
    cout<<LCS(s1,s2);
    return 0;
}
