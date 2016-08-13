#include <iostream>
#include<string.h>
#define max 100000
using namespace std;

int main()
{
    char str[max],ptr[20];
    int j,pos,m,n;
    cout<<"Enter The String:"<<endl;
    cin>>str;
    cout<<"Enter The Pattern To Be Searched:"<<endl;
    cin>>ptr;
    m=strlen(str);
    n=strlen(ptr);
    for(pos=0;pos<n-m;pos++)
    {
        j=0;
        while(j<m&&str[pos+j]==ptr[j])
        {
            j++;
        }

    }

      if(j>=m)
        cout<<pos;
      else
        cout<<"No Match Found"<<endl;
return 0;
}
