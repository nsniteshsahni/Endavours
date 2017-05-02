#include <bits/stdc++.h>
using namespace std;
int partition(int arr[],int p,int q)
{
    int i = p-1, j = p;
    int x = arr[q];
    while(j<=q-1)
    {
        if(x>arr[j])
        {
            i++;
            swap(arr[i],arr[j]);
        }
        j++;
    }
    swap(arr[i+1],arr[q]);
    return i+1;
}
void quicksort(int arr[],int p,int r)
{
    if(p<r)
    {
        int q = partition(arr,p,r);
        quicksort(arr,p,q-1);
        quicksort(arr,q+1,r);
    }
}
int main()
{
    int arr[]={7,3,4,6,8,2};
    quicksort(arr,0,5);
    for(int i=0;i<6;i++)
        cout<<arr[i]<<" ";
    return 0;
}
