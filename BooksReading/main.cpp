/*
You are given N books, ith of which has Pi pages (Pi <= Pj, if i < j). You have to assign these N books to M students, such that each student has subsegment of books and the maximum number of pages assigned to a student is minimized.
You have to find the maximum number of pages, a student can have in this assignment of books.


Input Format:
First line contains integers, N and M, denoting the number of books and number of students respectively. Next line contains N integers, denoting the number of pages in N books.

Constraints:
1<=N<=10^5
1<=M<=N
1<=Pi<=10^9
Output Format:
Print a single integer, denoting the answer.

Sample Input:
4 2
10 20 30 40
Sample Output:
60
*/

#include <bits/stdc++.h>
using namespace std;
#define ll long long

bool checkValid(vector <int> books,ll int n,ll int m,ll int mid)
{
      ll int temp = 1;
      ll int curr_pages=0;
      for(ll int i=0;i<n;i++)
      {
          if(curr_pages+books[i]>mid)
            {
               curr_pages = books[i];
               temp++;
               if(temp>m)
                    return false;
            }
          else
              curr_pages += books[i];
      }
   return true;
}
ll int BinarySearch(vector <int> books,ll int n,ll int m)
{
    ll int max_pages = 0, min_pages = 0,mid=0;

    for(ll int i=0;i<n;i++)
        max_pages = max_pages + books[i];

    min_pages = books[n-1];
    ll int ans = min_pages;
    while(min_pages<=max_pages)
    {
        mid = (max_pages+min_pages)/2;
        if(checkValid(books,n,m,mid))
        {
            ans = mid;
            max_pages = mid-1;
        }
        else
        {
            min_pages = mid+1;
        }
    }
   return ans;
}
int main()
{
    ll int n,m;
    cin>>n>>m;
    vector <int> books(n);
    for(int i=0;i<n;i++)
        cin>>books[i];
    cout<<BinarySearch(books,n,m)<<endl;
    return 0;
}
