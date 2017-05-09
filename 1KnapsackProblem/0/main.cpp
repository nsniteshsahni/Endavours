#include <bits/stdc++.h>
using namespace std;
int knapsack(int arr[],int p[],int size,int weight)
{
    int temp[size+1][weight+1];
    for(int i=0;i<size+1;i++)
    {
        for(int j=0;j<weight+1;j++)
        {
            if(i==0||j==0)
                temp[i][j]=0;
            else if(j>=arr[i-1])
                temp[i][j] = max(temp[i-1][j], p[i-1] + temp[i-1][j-arr[i-1]]);
            else
                temp[i][j] = temp[i-1][j];
        }
    }
return temp[size][weight];
}
int main()
{
    int arr[] = {1,2,4,7};
    int p[] = {3,4,5,6};
    cout<<knapsack(arr,p,4,10);
    return 0;
}
