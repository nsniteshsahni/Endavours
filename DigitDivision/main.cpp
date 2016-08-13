#include <iostream>
#include <string>
using namespace std;
int div(unsigned long long int x,unsigned long long int y)
{
    if(x%y==0)
    return 1;
    else
    return 0;
}
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
    string str;
    unsigned long long int i,jcount=0;
    cin>>str;
    for(i=0;i<str.length();i++)
    {
        if(div((int)*str,(int)str[i]))
           count++;

    }
    cout <<count<< endl;
    }
    return 0;
}
