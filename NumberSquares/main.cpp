#include <iostream>
#include <conio.h>
using namespace std;

int main()
{
    int a=1,b,n,i,j;
    cin>>n;
    b=(n-1)*4;
    for(i=1;i<=n;i++)
        {
            if(i==1)
            for(j=1;j<=n;j++)
            {
                cout<<a++<<" ";
            }
            else if(i<n)
                {
                    cout<<b--;
                    for(j=1;j<2*n-3;j++)
                        cout<<" ";
                        cout<<a++;
                }
        else
        {

        for(j=1;j<=n;j++)
        cout<<b--;
        }
        cout<<endl;
        }
    getch();
    return 0;
}
