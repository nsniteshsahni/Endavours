#include<iostream>
using namespace std;
void JobSequence(int A[], int S[], int F[],int n)
{
        int AS[10],k,m,i;
        for( i=1;i<=n;i++)
                AS[i]=0;
        AS[1]=A[1];
        k=1;
        i=1;
        for(m=2;m<=n;m++)
        {
                if(S[m]>=F[k])
                {
                        i++;
                        AS[i]=A[m];
                        k=m;
                }
        }

        cout<<" Following is the sequence of activities\n";
        for(int i=1;i<=n;i++)
        {
                if(AS[i]!=0)
                 cout<<AS[i]<<",";
        }
 }
int main()
{
        int A[10], S[10], F[10],n;
        cout<<"Enter the no. of activities";
        cin>>n;
        cout<<"Enter the Activity No., Start & End time"<<endl;
        cout<<" Act\tStart\tFinal\n";
        for(int i=1; i<=n;i++)
        {
                cin>>A[i];
                cin>>S[i];
                cin>>F[i];
        }
        cout<<"Your problem: \n"<<" Act\tStart\tFinal\n";
        for(int i=1; i<=n;i++)
        {
                cout<<A[i]<<"\t";
                cout<<S[i]<<"\t\t";
                cout<<F[i]<<"\t";
                cout<<"\n";
        }

        for(int i=2;i<=n;i++)
        for(int j=2;j<=i;j++)
        {
                if(F[j]<F[j-1])
                {
                        int t=A[j];
                        A[j]=A[j-1];
                        A[j-1]=t;
                        int x=S[j];
                        S[j]=S[j-1];
                        S[j-1]=x;
                        int y=F[j];
                        F[j]=F[j-1];
                        F[j-1]=y;
                }
        }
        cout<<"Your sorted problem: \n"<<" Act\tStart\tFinal\n";
        for(int i=1; i<=n;i++)
        {
                cout<<A[i]<<"\t";
                cout<<S[i]<<"\t\t";
                cout<<F[i]<<"\t";
                cout<<"\n";
        }
        JobSequence(A ,S,F, n);
        return 0;
}
