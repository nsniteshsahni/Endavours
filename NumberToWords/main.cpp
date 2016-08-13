#include<bits/stdc++.h>
using namespace std;
void expand(int num)
{
const char* ones[20]={"zero", "one","two","three","four","five","six","seven","eight","nine", "eleven","tweleve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","ninteen"};
const char* tens[10]={" ","ten","twenty","thirty","fourty","fifty","sixty","seventy","eighty","ninty"};
if(num<0)
{
   cout<<"minus";
   expand(-num);
}
else if(num>=1000)
{

   expand(num/1000);
   cout<<" thousand ";

   if(num%1000)
  {
    if(num%1000<100)
    {
      cout<<" and ";
    }
      cout<<" ";
      expand(num%1000);
    }
}
else if(num>=100)
{
   expand(num/100);
   cout<<" hundred ";
   if(num%100)
   {
     cout<<" and ";
     expand(num%100);
   }
}
else if(num>=20)
{
   cout<<tens[num/10];
   if(num%10)
   {
     cout<<" ";
     expand(num%10);
   }
}
else
{
   cout<<ones[num];
}
return;
}
int main()
{
int n;
cout<<"Enter The Number\n";
cin>>n;
expand(n);
}
