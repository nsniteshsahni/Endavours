#include <iostream>
using namespace std;
int myXOR(int x,int y)
    {
        return ((x&~y)|(~x&y));
    }

int main()
{
    int a,b;
    cin>>a>>b;
    cout<<myXOR(a,b);
    return 0;
}
