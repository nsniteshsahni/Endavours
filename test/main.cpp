#include<iostream>
#include <stdio.h>
#include <string.h>
using namespace std;
int main()
{
  int t,k;
  cin>>t;
  for(k=0;k<t;k++)
  {
  char str[30],str1[30];
  cin>>str;
  strcpy(str1,str);
  int len = strlen(str);
  int read_pos, write_pos, prev_char,count=0;

  prev_char = str[0] + 1;
  	for (read_pos = 0, write_pos = 0; read_pos < len; read_pos++)
  {
    if (str[read_pos] != prev_char)
    {
      str[write_pos] = str[read_pos];
      write_pos++;
    }
    prev_char = str[read_pos];
  }
  str[write_pos] = '\0';
if(strcmp(str,str1)!=0)
  printf("%s\n", str);
  else
  cout<<str1;
  }
  return 0;
}
