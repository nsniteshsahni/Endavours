#include <iostream>
#include <conio.h>
using namespace std;
int max(int x,int y)
{
    if(x>y)
    return x;
    else
    return y;
}

int main()
{
    int A[5][5],i,j,sum=0,n;
    cout<<"Enter The Size"<<endl;
    cin>>n;
    for(i=0;i<n;i++)
       for(j=4;j>i;j--)
     A[i][j]=0;
    cout<<"Enter The values"<<endl;
    for(i=0;i<n;i++)
       for(j=0;j<=i;j++)
            cin>>A[i][j];
    sum=sum+A[0][0];
    if(n>=2)
    {
        sum=sum+max(A[1][0],A[1][1]);
        if(n>=3)
        {
            sum=sum+max(max(A[2][0],A[2][1]),A[2][2]);
        }
        if(n>=4)
        {
            sum=sum+max(max(A[3][0],A[3][1]),max(A[3][2],A[3][3]));
        }
        if(n>=5)
        {
            sum=sum+max(max(max(A[4][0],A[4][1]),max(A[4][2],A[4][3])),A[4][4]);
        }
    }
    cout<<"The Max Traversal Cost Is: "<<sum<<endl;
    getch();
    return 0;
}

