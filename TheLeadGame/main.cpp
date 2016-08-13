#include<iostream>
using namespace std;
int main()
{
	int n, s, t, w, l, max, i;
	cin >> n;
	max = 0;
	for (i = 0; i < n; i++)
	{
		cin >> s >> t;
		if (s>t)
		{
			l = s - t;
			if (l > max)
			{
				max = l;
				w = 1;
			}
		}
		else
		{
			l = t - s;
			if (l > max)

			{
				max = l;
				w = 2;
			}
		}
	}
	cout<< "\n"<<w<<" "<<max<<endl;
	return 0;
}
