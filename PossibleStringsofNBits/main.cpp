#include <bits/stdc++.h>
using namespace std;
char str[1000];
void PrintStrins(int n)
{
    if(n==0)
        cout<<str<<endl;
     else
     {
        str[n-1]='0';
        PrintStrins(n-1);
        str[n-1]='1';
        PrintStrins(n-1);
     }
}
int main()
{
    PrintStrins(3);
    return 0;
}
