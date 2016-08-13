// prime nos btw two numbers.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<math.h>
using namespace std;

int main()
{   unsigned long int test,k;
    int x, y, i, flag;
cin>>test;
	for(k=0;k<test;k++)
    {
        cin >> x >> y;
  while (x<=y)
	{
		flag = 0;
		if ((x == 2) || (x == 3))
		{
			cout << x << endl;
			x = x + 1;
			continue;
		}

		for (i = 2; i <=sqrt(x); i++)
		{
			if ((x%i) == 0)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 0)
		{
         if(x!=1)
            cout<<x<<endl;
		}
		x = x + 1;
	}


    }
return 0;
}
