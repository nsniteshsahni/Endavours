#include <bits/stdc++.h>
using namespace std;
int MinPathCost(int arr[3][4],int m,int n)
{
 int cost[3][4];
 cost[0][0]=arr[0][0];

 for (int i=1;i<n;i++)
    cost[0][i] = cost[0][i-1] + arr[0][i];

 for (int i=1;i<m;i++)
    cost[i][0] = cost[i-1][0] + arr[i][0];

 for (int i=1;i<m;i++)
 {
     for(int j=1;j<n;j++)
     {
         cost[i][j] = arr[i][j] + min(min(cost[i-1][j],cost[i][j-1]),cost[i-1][j-1]);
     }
 }
 return cost[m-1][n-1];
}
int main()
{
    int arr[3][4]= { {1,4,3,9},{5,3,4,9},{4,1,7,6}};
    cout<<MinPathCost(arr,3,4);
    return 0;
}
