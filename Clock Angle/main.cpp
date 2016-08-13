    #include <iostream>
    #include <cmath>
    using namespace std;
     
    int main()
    {
        int t,h,m;
        cin>>t;
        while(t--)
        {
            int angle,hour,min;
        	cin>>h>>m;
        	hour=30*h+m/2;
        	min=6*m;
        	angle=abs(hour-min);
        	if(angle>180)
            {
        		cout<<360-angle<<endl;
        	}
        	else
            {
        	cout<<angle<<endl;
            }
        }
        return 0;
    }