#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
using namespace std;

int main()
{
    unsigned long long int i,j,k;
    srand( (unsigned)time( NULL ) );
    ofstream fout;
    ifstream fin;
    //fout.open("test.txt",ios::out);
    /*for(i=0;i<100;i++)
    {
      j=rand();
      fout<<j<<endl;
    }
 fout.close(); */
    fin.open("test.txt",ios::in);
    fout.open("out.txt",ios::out);
    for(i=0;i<100;i++)
      {
          fin>>j;
          if(j%2==0)
            fout<<"EVEN"<<endl;
          else
            fout<<"ODD"<<endl;
      }
    return 0;
}
