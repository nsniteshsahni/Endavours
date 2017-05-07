#include <bits/stdc++.h>
using namespace std;

void solve(int arr[],int size)
{
 stack <int> s;
 s.push(arr[0]);
 int temp;
 for(int i=1;i<size;i++)
 {
     if(!s.empty())
     {
      temp = s.top();
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
		printf("%d -> -1\n", s.top());
		s.pop();
	}
}

int main()
{
    int arr[] = {13,7,6,12};
    solve(arr,4);
    return 0;
}
