#include <bits/stdc++.h>
#define ROWS 4
#define COLS 4

using namespace std;

int isSafe(int maze[ROWS][COLS],int x,int y)
{
    if(x>=0&&x<ROWS&&y>=0&&y<COLS&&maze[x][y]==1)
        return 1;
    else
        return 0;
}

int compute(int maze[ROWS][COLS],int x,int y,int solution[ROWS][COLS])
{
   if(x == ROWS-1 && y == COLS-1)
   {
       solution[x][y]=1;
       return 1;
   }
   if(isSafe(maze,x,y))
   {
       solution[x][y]=1;
       if(compute(maze,x+1,y,solution)||compute(maze,x,y+1,solution))
         return 1;
       solution[x][y]=0;
   }
   return 0;
}
void print(int solution[ROWS][ROWS])
{
    for(int i=0;i<ROWS;i++)
    {
        for(int j=0;j<COLS;j++)
        {
            cout<<solution[i][j]<<" ";
        }
        cout<<endl;
    }
}
void solve(int maze[ROWS][COLS])
{
    int solution [ROWS][COLS] = {{0, 0, 0, 0},
                                 {0, 0, 0, 0},
                                 {0, 0, 0, 0},
                                 {0, 0, 0, 0}
                               };
    if(!compute(maze,0,0,solution))
        cout<<"Solution not possible";
    else
        print(solution);
}

int main()
{
    int maze[ROWS][COLS]=    { {1, 1, 0, 0},
                               {1, 1, 1, 1},
                               {0, 0, 1, 0},
                               {1, 1, 1, 1}
                             };
    solve(maze);
    return 0;
}
