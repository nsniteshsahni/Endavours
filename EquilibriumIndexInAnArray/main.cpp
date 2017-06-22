#include <bits/stdc++.h>
using namespace std;
int solve(int arr[],int size)
{
    int sum=0,leftsum=0;
    for(int i=0;i<size;i++)
        sum +=arr[i];

    for(int i=0;i<size;i++)
    {
        sum-=arr[i];
        if(leftsum==sum)
            return i;
        leftsum+=arr[i];
    }
    return -1;
}
int main()
{
    int arr[] = {-7, 1, 5, 2, -4, 3, 0};
    cout<<solve(arr,7);
    return 0;
}
