#include <bits/stdc++.h>
using namespace std;

void solve(int arr[],int size)
{
 stack <int> s;
 s.push(arr[0]);
 for(int i=1;i<size;i++)
 {
     if(!s.empty())
     {
     int temp = s.top();
     s.pop();
     while(temp<arr[i])
        {
          cout<<temp<<" -> "<<arr[i]<<endl;
          if(s.empty())
            break;
          temp = s.top();
          s.pop();

        }
        if(temp>arr[i])
          s.push(temp);
     }
       s.push(arr[i]);
 }
  while (!s.empty())
	{
		printf("%d -> -1", s.top());
		s.pop();
	}
}

int main()
{
    int arr[] = {4,5,2,25};
    solve(arr,4);
    return 0;
}
