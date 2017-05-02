#include <bits/stdc++.h>
using namespace std;
void InsertionSort(int arr[],int size)
{
    int j=1;
    while(j<size)
    {
        int key = arr[j];
        int i=j-1;
        while(i>=0&&arr[i]>key)
        {
            arr[i+1]=arr[i];
            i--;
            arr[i+1]=key;
        }
        j++;
    }

    for(int i=0;i<size;i++)
        cout<<arr[i]<<" ";
}
int main()
{
    int arr[]={5,6,8,7,1,4};
    InsertionSort(arr,6);
    return 0;
}
