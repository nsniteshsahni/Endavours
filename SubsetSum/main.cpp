#include <bits/stdc++.h>

using namespace std;
int solve(int arr[],int sum,int size)
{
    int temp[size+1][sum+1];
    for(int i=0;i<size+1;i++)
    {
        for(int j=0;j<sum+1;j++)
        {
            if(i==0)
                temp[i][j] = 0;
            else if(j==0)
                temp[i][j] = 1;
            else if(j<arr[i])
                temp[i][j] = temp[i-1][j];
            else if(j>=arr[i])
                temp[i][j] = temp[i][j-arr[i-1]]||temp[i-1][j];
        }
    }
return temp[size][sum];
}
int main()
{
    int arr[]={5,3,4};
    int sum=7;
    cout<<solve(arr,sum,3);
    return 0;
}
