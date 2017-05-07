#include <bits/stdc++.h>
#define SIZE 4

using namespace std;

int isSafe(int board[SIZE][SIZE],int x,int y)
{
    int i,j;
    for(i=0;i<y;i++)
    {
        if(board[x][i])
            return 0;
    }
    for(i=x,j=y;i>=0&&j>=0;i--,j--)
    {
        if(board[i][j])
            return 0;
    }
    for(i=x,j=y;i<SIZE&&j>=0;i++,j--)
    {
        if(board[i][j])
            return 0;
    }
    return 1;
}

int Solve(int board[SIZE][SIZE],int y)
{
    if(y>=SIZE)
        return 1;
    for(int i=0;i<SIZE;i++)
    {
       if(isSafe(board,i,y))
       {
        board[i][y]=1;
        if(Solve(board,y+1))
            return 1;
        board[i][y]=0;
       }
    }
    return 0;
}
int main()
{
    int board[SIZE][SIZE]={{0,0,0,0},
                           {0,0,0,0},
                           {0,0,0,0},
                           {0,0,0,0}
                          };
    if(!Solve(board,0))
        cout<<"Solution not possible";
    else
        {
        for(int i=0;i<SIZE;i++)
          {
             for(int j=0;j<SIZE;j++)
             cout<<board[i][j];
            cout<<endl;
          }

        }
    return 0;
}
