#include <bits/stdc++.h>
using namespace std;
int CoinChange(int value,int coins[],int size)
{
    int temp[size+1][value+1];
    for(int i=0;i<size+1;i++)
    {
        for(int j=0;j<value+1;j++)
        {
            if(i==0)
                temp[i][j]=0;
            else if(j==0)
                temp[i][j]=1;
            else if(j-coins[i-1]>=0)
            {
                temp[i][j] = temp[i-1][j] + temp[i][j-coins[i-1]];
            }
            else
                temp[i][j]=temp[i-1][j];
        }
    }
    return temp[size][value];
}
int main()
{
    int value = 3;
    int coins[]={8,1,2,3};
    cout<<CoinChange(value,coins,4);
    return 0;
}
