#include <iostream>
#include<conio.h>
using namespace std;

int main()
{  unsigned long int i=0,d[7],co,k;
 long int m,n,num,key,ans;
 cout<<"\nEnter the no of test cases\n";
cin>>n;
for(m=0;m<n;m++)
{
    co=0;
    cout<<"\nEnter The Number:\n";
    cin>>num;
    key=num;
    if(num==1||num==4||num==27||num==256||num==3125||num==46656||num==823543)
    {
         co++;
         goto a;
    }
while (num>0)
{

d[i]=num%10;
num=num/10;
i++;
}

    for(k=0;k<i;k++)
    {

     ans=(key-(d[k]*d[k]));
    if((ans==1||ans==4||ans==27||ans==256||ans==3125||ans==46656||ans==823543))
    {
        co++;
        goto a;
    }
    }
a:
{if(co==0)
    cout<<"No";
else
    cout<<"Yes";
}
}
getch();
return 0;
}
