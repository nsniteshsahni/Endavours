#include<stdio.h>
#include<conio.h>
//GLOBAL VARIABLES - DECLARATION
int Twt,Ttt,n,A[20],Wt[20],Bu[20];
float Att,Awt;
char pname[20][20];
//FUNCTION DECLARATIONS
void Getdata();
void Gantt_chart();
void Sjf();
//GETTING THE NUMBER OF PROCESSES AND THE BURST TIME AND ARRIVAL TIME FOR EACH PROCESS
void Getdata()
{
	int i;
	printf("\n Enter the number of processes: ");
	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		fflush(stdin);
		printf("\n\n Enter the process name: ");
		scanf("%s",&pname[i]);
		printf("\n Enter The BurstTime for Process %s =	",pname[i]);
		scanf("%d",&Bu[i]);
		printf("\n Enter the Arrival Time for Process %s =	",pname[i]);
		scanf("%d",&A[i]);
	}
}
//DISPLAYING THE GANTT CHART
void Gantt_chart()
{
	int i;
	printf("\n\nGANTT CHART");
	printf("\n--------------------------------------------------------------------\n");
	for(i=1;i<=n;i++)
		printf("|\t%s\t",pname[i]);
	printf("|\t\n");
	printf("\n-----------------------------------------------------------\n");
	printf("\n");
	for(i=1;i<=n;i++)
		printf("%d\t\t",Wt[i]);
	printf("%d",Wt[n]+Bu[n]);
	printf("\n--------------------------------------------------------------------\n");
	printf("\n");
 }
//Shortest job First Algorithm with NonPreemption
void Sjf()
{
	int w,t,i,B[10],Tt=0,temp,j;
	char S[10],c[20][20];
	int temp1;
	printf("\n\n SHORTEST JOB FIRST SCHEDULING ALGORITHM \n\n");
	Twt=Ttt=0;
	w=0;
	for(i=1;i<=n;i++)
	{
		B[i]=Bu[i];
		S[i]='T';
		Tt=Tt+B[i];
	}
	for(i=1;i<=n;i++)
	{
		for(j=3;j<=n;j++)
		{
			if(B[j-1]>B[j])
			{
				temp=B[j-1];
				temp1=A[j-1];
				B[j-1]=B[j];
				A[j-1]=A[j];
				B[j]=temp;
				A[j]=temp1;
				strcpy(c[j-1],pname[j-1]);
				strcpy(pname[j-1],pname[j]);
				strcpy(pname[j],c[j-1]);
			}
		}
	}
	//For the 1st process
	Wt[1]=0;
	w=w+B[1];
	t=w;
	S[1]='F';
	while(w<Tt)
	{
		i=2;
		while(i<=n)
		{
			if(S[i]=='T'&&A[i]<=t)
			{
				Wt[i]=w;
				S[i]='F';
				w=w+B[i];
				t=w;
				i=2;
			}
			else
				i++;
		}
	}
	//CALCULATING AVERAGE WAITING TIME AND AVERAGE TURN AROUND TIME
	for(i=1;i<=n;i++)
	{
		Twt=Twt+(Wt[i]-A[i]);
		Ttt=Ttt+((Wt[i]+Bu[i])-A[i]);
	}
	Att=(float)Ttt/n;
	Awt=(float)Twt/n;
	printf("\n\n Average Turn around time=%3.2f ms ",Att);
	printf("\n\n AverageWaiting Time=%3.2f ms",Awt);
	Gantt_chart();
}
void main()
{
	//clrscr();
	Getdata();
	Sjf();
	getch();
}
