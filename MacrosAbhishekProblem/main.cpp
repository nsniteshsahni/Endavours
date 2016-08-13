#include <iostream>
using namespace std;
#define getch()  100
#define clrscr() 200
int main()
{
int count_of_avengers = clrscr() + ( ~ (getch() ) ) +1;
cout<<count_of_avengers;
return 0;
}
