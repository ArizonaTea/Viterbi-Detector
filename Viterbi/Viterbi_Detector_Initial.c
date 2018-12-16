/*
Viterbi Detector
Finite_State_Machine-AWGN_Channel-Vitervi_Detector
Author: Tao Liu
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int N,M;

#define N 11
#define M 10000000

long double Distance[4],DistanceM[4];
int Path[4][N+1],Path1[4][N+1];


main() {
int i,j,k,l,m,o,p,s;
int MesKit[N+1],Stat[N+1],Output[N+1];
float gen_rand_uniform(); //function for generating uniform [0,1) random numbers
float gaussrand();
int *Viterbi_Detector(float Receive1[N+1]);
float pi;
int b;
pi=3.141592654;

/*--------------------------------Filled the checking block-------------------------------*/

for(i=1;i<=N;i++)
{
 /* generating random bits 0, 1 */
 //b = (gen_rand_uniform()>0.5)?1:0;
 /* storing the bits, reordering the bits in the memory */
 MesKit[1]=1;
 MesKit[2]=1;
 MesKit[3]=1;
 MesKit[4]=0;
 MesKit[5]=0;
 MesKit[6]=0;
 MesKit[7]=0;
 MesKit[8]=0;
 MesKit[9]=1;
 MesKit[10]=1;
 MesKit[11]=1;
}
/* Finiate State Machine */
Stat[1]=3;
for(j=1;j<=N;j++)
{
	if ((MesKit[j]==0) && (Stat[j]==0)) 
		Stat[j+1]=0;
	if ((MesKit[j]==0) && (Stat[j]==1)) 
		Stat[j+1]=2;
	if ((MesKit[j]==0) && (Stat[j]==2)) 
		Stat[j+1]=0;
	if ((MesKit[j]==0) && (Stat[j]==3)) 
		Stat[j+1]=2;
	if ((MesKit[j]==1) && (Stat[j]==0)) 
		Stat[j+1]=1;
	if ((MesKit[j]==1) && (Stat[j]==1)) 
		Stat[j+1]=3;
	if ((MesKit[j]==1) && (Stat[j]==2)) 
		Stat[j+1]=1;
	if ((MesKit[j]==1) && (Stat[j]==3)) 
		Stat[j+1]=3;			
}
for(k=1;k<=N;k++)
{
    if ((MesKit[k]==0) && (Stat[k]==0)) 
		Output[k]=-2;
	if ((MesKit[k]==0) && (Stat[k]==1)) 
		Output[k]=0;
	if ((MesKit[k]==0) && (Stat[k]==2)) 
		Output[k]=-1;
	if ((MesKit[k]==0) && (Stat[k]==3)) 
		Output[k]=1;
	if ((MesKit[k]==1) && (Stat[k]==0)) 
		Output[k]=-1;
	if ((MesKit[k]==1) && (Stat[k]==1)) 
		Output[k]=1;
	if ((MesKit[k]==1) && (Stat[k]==2)) 
		Output[k]=0;
	if ((MesKit[k]==1) && (Stat[k]==3)) 
		Output[k]=2;	
}

/*AWGN Channel*/
float noise;
float Receive[N+1];
for(l=1;l<=N;l++)
{
 Receive[l]=0;
 noise=gaussrand();
 Receive[l]=Output[l]+0*noise; 
 //Receive[l]=Output[l];
}

/*Viterbi Detector*/
int Pathfinal[N+1];
int *ii;
ii=Viterbi_Detector(Receive);
for(m=0;m<=N;m++)
	Pathfinal[m]=ii[m];

/*Detection*/
int ReceBits[N+1];

for(o=0;o<N;o++)
{
 if ((Pathfinal[o]==0) && (Pathfinal[o+1]==0))
 	ReceBits[o+1]=0;
 if ((Pathfinal[o]==0) && (Pathfinal[o+1]==1))
 	ReceBits[o+1]=1;
 if ((Pathfinal[o]==1) && (Pathfinal[o+1]==2))
 	ReceBits[o+1]=0;
 if ((Pathfinal[o]==1) && (Pathfinal[o+1]==3))
 	ReceBits[o+1]=1;
 if ((Pathfinal[o]==2) && (Pathfinal[o+1]==0))
 	ReceBits[o+1]=0;
 if ((Pathfinal[o]==2) && (Pathfinal[o+1]==1))
 	ReceBits[o+1]=1;
 if ((Pathfinal[o]==3) && (Pathfinal[o+1]==2))
 	ReceBits[o+1]=0;
 if ((Pathfinal[o]==3) && (Pathfinal[o+1]==3))
 	ReceBits[o+1]=1;
}

/*BER calculation*/
int Error;
float BER;
Error=0;
for(p=1;p<=N;p++)
{
 if (MesKit[p]==ReceBits[p])
 	Error=Error;
 else
 	Error=Error+1;	
}
BER=Error/N;
/*Test*/

/*for(s=1;s<=100;s++)
	printf("%d\n",MesKit[s]);*/
for(s=0;s<=N;s++)
{
printf("%d %d %f %d %d %d\n",Stat[s+1],Output[s+1],Receive[s+1],MesKit[s+1],ReceBits[s+1],Error);
}	

for(s=0;s<=N;s++)
{
printf("%d %d %d %d\n",Path[0][s],Path[1][s],Path[2][s],Path[3][s]);
}



}

float gen_rand_uniform() {
float u1;
u1 = (double)rand()/((double)RAND_MAX + (double)1); 
return(u1);
}

float gaussrand()
{
     float V1,V2,S;
     int phase= 0;
     float X;

     if(phase==0)
     {
        do{
              float U1=(float)rand()/RAND_MAX;
              float U2=(float)rand()/RAND_MAX;
              
              V1=2*U1-1;
              V2=2*U2-1;
              S=V1*V1+V2*V2;
          }while(S>=1||S==0);
    
          X=V1*sqrt(-2*log(S)/S);
     }
     else
     {
          X=V2*sqrt(-2*log(S)/S);
     }

     phase=1-phase;
     return(X);
}

int * Viterbi_Detector(float Receive1[N+1])
{
	float Distance1,Distance2,min;
	int m,n,p,num,q;
	static int Pathfinal1[N+1];
	
	
	Path[0][0]=0;
	Path[1][0]=1;
	Path[2][0]=2;
	Path[3][0]=3;
	
	Distance[0]=0;
	Distance[1]=0;
	Distance[2]=0;
	Distance[3]=0;
	
	for(m=1;m<=N;m++)
	{
	
	DistanceM[0]=Distance[0];
	DistanceM[1]=Distance[1];
	DistanceM[2]=Distance[2];
	DistanceM[3]=Distance[3];
	
	for(p=0;p<=m-1;p++)
	{
	 Path1[0][p]=Path[0][p];
	 Path1[1][p]=Path[1][p];
	 Path1[2][p]=Path[2][p];
	 Path1[3][p]=Path[3][p];
	}
	
	Distance1=DistanceM[0]+(Receive1[m]+2)*(Receive1[m]+2);
	Distance2=DistanceM[2]+(Receive1[m]+1)*(Receive1[m]+1);
	
	if (Distance1<Distance2)
	{
		
		Path[0][m]=0;
		Distance[0]=DistanceM[0]+Distance1;
	}
	else
	{
		for(n=0;n<=m-1;n++)
		{
		 Path[0][n]=Path1[2][n];
		}
		Path[0][m]=0;
		Distance[0]=DistanceM[2]+Distance2;
	}
	
	Distance1=DistanceM[0]+(Receive1[m]+1)*(Receive1[m]+1);
	Distance2=DistanceM[2]+(Receive1[m]-0)*(Receive1[m]-0);
	
	if (Distance1<Distance2)
	{
		for(n=0;n<=m-1;n++)
		{
		 Path[1][n]=Path1[0][n];
		}
		Path[1][m]=1;
		Distance[1]=DistanceM[0]+Distance1;
	}
	else
	{
		for(n=0;n<=m-1;n++)
		{
		 Path[1][n]=Path1[2][n];
		}
		Path[1][m]=1;
		Distance[1]=DistanceM[2]+Distance2;
	}
	
	Distance1=DistanceM[1]+(Receive1[m]-0)*(Receive1[m]-0);
	Distance2=DistanceM[3]+(Receive1[m]-1)*(Receive1[m]-1);
	
	if (Distance1<Distance2)
	{
		for(n=0;n<=m-1;n++)
		{
		 Path[2][n]=Path1[1][n];
		}
		Path[2][m]=2;
		Distance[2]=DistanceM[1]+Distance1;
	}
	else
	{
		for(n=0;n<=m-1;n++)
		{
		 Path[2][n]=Path1[3][n];
		}
		Path[2][m]=2;
		Distance[2]=DistanceM[3]+Distance2;
	}
	
	Distance1=DistanceM[1]+(Receive1[m]-1)*(Receive1[m]-1);
	Distance2=DistanceM[3]+(Receive1[m]-2)*(Receive1[m]-2);
	
	if (Distance1<Distance2)
	{
		for(n=0;n<=m-1;n++)
		{
		 Path[3][n]=Path1[1][n];
		}
		Path[3][m]=3;
		Distance[3]=DistanceM[1]+Distance1;
	}
	else
	{
		Path[3][m]=3;
		Distance[3]=DistanceM[3]+Distance2;
	}
	
	}
	
	min=Distance[0];
	num=0;
		for(q=1;q<=3;q++)
		{
		if (min>Distance[q])
			{
			min=Distance[q];
			num=q;
			}
		}
for(p=0;p<=N;p++)
{
 Pathfinal1[p]=Path[num][p];
}

return &Pathfinal1[0];
}


