#include <bits/stdc++.h>
using namespace std;
int FindElement(int arr[],int low,int high,int value)
{
    int mid = (low+high)/2;
    if(arr[mid]==value)
        return mid;
    else if (arr[mid+1]>arr[mid] && value>arr[mid]&&arr[high]>=value)
        FindElement(arr,mid+1,high,value);
    else if (arr[mid-1]>arr[mid]&&value>=arr[low])
        FindElement(arr,0,mid-1,value);
    else
        return -1;
}
int main()
{
    int arr[]={7,8,9,10,1,2,3,4,5,6};
    cout<<FindElement(arr,0,9,45);
    return 0;
}
