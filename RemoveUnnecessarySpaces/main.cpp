#include <iostream>
#include<string>
using namespace std;

int main()
{
    string s="Hello                    World          This     is          Google",p;
    int i=0,j=0,len=0;
    len=s.length();
 while(i<len)
    {
     if(s[i]!=' ')
     {
         if(i!=j)
         {
         p.append(" ");
         j=i;
         }
     else
     {
         string temp;
         temp=s[i];
         p.append(temp);
         i++;
         j++;
     }
     }
    else
    {
        i++;
    }
   }
 cout<<p<<endl;
    return 0;
}
