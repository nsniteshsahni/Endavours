#include <bits/stdc++.h>
using namespace std;
void MaxSubArray(int arr[],int size)
{
  int max_sum_so_far = 0,curr_sum = 0,index = 0;
  for(int i = 0; i< size ;i++)
  {
      curr_sum += arr[i];
      if(curr_sum<0)
        curr_sum = 0;
      if(curr_sum > max_sum_so_far)
      {
          max_sum_so_far = curr_sum;
          index = i;
      }

  }
cout<<max_sum_so_far<<endl;
int start=index, end = index,sum = max_sum_so_far;
while(sum!=0)
{
    sum-=arr[start];
    start--;
}
cout<<"Subarray starts from "<<start+1<<" and ends at "<<end;
}
int main()
{
    int arr[]={-2, -3, 4, -1, -2, 1, 5, -3};
    MaxSubArray(arr,8);
    return 0;
}
