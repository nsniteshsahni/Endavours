// Holes in the text.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<conio.h>

using namespace std;

int main()
{
	char a[100];
	int t, i, j, h;
	cout << "enter the no of test cases" << endl;
	cin >> t;
	for (i = 0; i < t; i++)
	{
		cout << "Enter the string in capital letters" << endl;
		cin >> a;
		h = 0;
		for (j = 0; a[j] != '\0'; j++)
		{
			if ((a[j] == 'A') || (a[j] == 'D') || (a[j] == 'O') || (a[j] == 'P') || (a[j] == 'Q') || (a[j] == 'R'))
				h++;
			if (a[j] == 'B')
			    h = h + 2;
		}
		cout << "the total no of holes is\t" << h << endl;
	}
	_getch();
	return 0;
}
