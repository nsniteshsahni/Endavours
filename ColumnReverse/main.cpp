#include <bits/stdc++.h>

using namespace std;

int main()
{
    //cout << "Hello world!" << endl;
    int A[3][5]={{15,12,56,45,51},{13,91,92,87,63},{11,23,61,46,81}};
    for(int i=2;i>=0;i--)
    {
        for(int j=0;j<5;j++)
        {
          cout<<A[i][j]<<" ";
        }
    cout<<endl;
    }
    return 0;
}
