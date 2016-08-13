#include <bits/stdc++.h>
using namespace std;

int main()
{
    //cout << "Hello world!" << endl;
    string str;
    int ans=0,b=2;
    cin>>str;
    reverse(str.begin(),str.end());
    for(int i=0;i<str.length();i++)
    {
        ans=ans+(int)str[i]*b;
        b=b*2;
    }
    //cout<<ans<<endl;
    cout<<
    return 0;
}
