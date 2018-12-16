/*
Viterbi Detector
Finite_State_Machine-AWGN_Channel-Vitervi_Detector
Author: Tao Liu
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int N,M;

#define N 5
#define M 100000000

float Distance[4],DistanceM[4];
int Path[4][N+2],Path1[4][N+2];

main() {
int i,j,k,l,m,o,p,q,r,s;
int MesKit[N+1],Stat[N+1],Output[N+1];
float gen_rand_uniform(); //function for generating uniform [0,1) random numbers
float gaussrand();
int *Viterbi_Detector_Initial(float Receive1[N+1]);
int Viterbi_Detector(float ReceiveMes1);
float pi,sigma;
int b;
pi=3.141592654;
sigma=0.3;

/*--------------------------------Filled_the_checking_block-------------------------------*/

for(i=1;i<=N;i++)
{
 /* generating random bits 0, 1 */
 b = (gen_rand_uniform()>0.5)?1:0;
 /* storing the bits, reordering the bits in the memory */
 MesKit[i]=b;
}
/* Finiate State Machine */
Stat[1]=0;
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
 noise=gaussrand();
 Receive[l]=Output[l]+sigma*noise;
 //Receive[l]=Output[l];
}

/*Viterbi Detector*/

Path[0][0]=0;
Path[1][0]=1;
Path[2][0]=2;
Path[3][0]=3;

Distance[0]=0;
Distance[1]=0;
Distance[2]=0;
Distance[3]=0;

int *qq;
qq=Viterbi_Detector_Initial(Receive);

/*-----------------------------------Decode_The_Information_Sequence----------------------------*/

int Message,Message1,Lmessage,State,Lstate,Output1,RecBit,Error;
float ReceiveMes;
Error=0;

for(r=N;r<M+N;r++)
{
    Message=(gen_rand_uniform()>0.5)?1:0;

    if (r==N)
	{
	Lmessage=MesKit[N];
	Lstate=Stat[N];
    }

    if ((Lmessage==0) && (Lstate==0)) 
		State=0;
	if ((Lmessage==1) && (Lstate==0)) 
		State=1;
	if ((Lmessage==0) && (Lstate==1)) 
		State=2;
	if ((Lmessage==1) && (Lstate==1)) 
		State=3;
	if ((Lmessage==0) && (Lstate==2)) 
		State=0;
	if ((Lmessage==1) && (Lstate==2)) 
		State=1;
	if ((Lmessage==0) && (Lstate==3)) 
		State=2;
	if ((Lmessage==1) && (Lstate==3)) 
		State=3;
				
    if ((Message==0) && (State==0)) 
		Output1=-2;
	if ((Message==1) && (State==0)) 
		Output1=-1;
	if ((Message==0) && (State==1)) 
		Output1=0;
	if ((Message==1) && (State==1)) 
		Output1=1;
	if ((Message==0) && (State==2)) 
		Output1=-1;
	if ((Message==1) && (State==2)) 
		Output1=0;
	if ((Message==0) && (State==3)) 
		Output1=1;
	if ((Message==1) && (State==3)) 
		Output1=2;

 ReceiveMes=0;
 noise=gaussrand();
 ReceiveMes=Output1+sigma*noise;
 //printf("%f\n",noise);
 //ReceiveMes=Output1;

RecBit=Viterbi_Detector(ReceiveMes);

for(q=1;q<=N;q++)
{
Receive[q]=Receive[q+1];
}
Receive[N]=ReceiveMes;


Message1=MesKit[1];
for(q=1;q<=N;q++)
{
MesKit[q]=MesKit[q+1];
}
MesKit[N]=Message;


if (RecBit==Message1)
	Error=Error;
else
	Error=Error+1;

//printf("%f %d %d\n",ReceiveMes,RecBit,Message1);

Lmessage=Message;
Lstate=State;

}

/*--------------------------------------Detection_Of_The_Last_N_Bits-------------------------------------*/
int Pathfinal[N+1];
int *ii;
ii=Viterbi_Detector_Initial(Receive);
for(m=0;m<=N;m++)
	Pathfinal[m]=ii[m];

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
float BER;
//for(p=1;p<=N;p++)
//{
// if ((MesKit[p])==(ReceBits[p]))
// 	Error=Error;
// else
// 	Error=Error+1;
//printf("%d %d\n",MesKit[p],ReceBits[p]);
//}

BER=Error/((float) (M));
/*Test*/

printf("%f\n",BER);
printf("%d\n",M);

/*for(s=1;s<=100;s++)
	printf("%d\n",MesKit[s]);*/
/*for(s=0;s<=N;s++)
{
printf("%d %d %f %d %d %d\n",Stat[s+1],Output[s+1],Receive[s+1],MesKit[s+1],ReceBits[s+1],Error);
}*/

printf("%d\n",Error);

}

float gen_rand_uniform() 
{
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

int * Viterbi_Detector_Initial(float Receive1[N+1])
{
	float Distance1,Distance2,min;
	int m,n,p,num;
	static int Pathfinal1[N+1];	
	
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
		Distance[0]=Distance1;
	}
	else
	{
		for(n=0;n<=m-1;n++)
		{
		 Path[0][n]=Path1[2][n];
		}
		Path[0][m]=0;
		Distance[0]=Distance2;
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
		Distance[1]=Distance1;
	}
	else
	{
		for(n=0;n<=m-1;n++)
		{
		 Path[1][n]=Path1[2][n];
		}
		Path[1][m]=1;
		Distance[1]=Distance2;
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
		Distance[2]=Distance1;
	}
	else
	{
		for(n=0;n<=m-1;n++)
		{
		 Path[2][n]=Path1[3][n];
		}
		Path[2][m]=2;
		Distance[2]=Distance2;
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
		Distance[3]=Distance1;
	}
	else
	{
		Path[3][m]=3;
		Distance[3]=Distance2;
	}
	
	}

      min=Distance[0];
	  num=0;
	  for(p=1;p<=3;p++)
	  {
	  if (min>Distance[p])
		  {
		  min=Distance[p];
		  num=p;
		  }
	  }
	
for(p=0;p<=N;p++)
{
 Pathfinal1[p]=Path[num][p];
}

return &Pathfinal1[0];
}

int Viterbi_Detector(float ReceiveMes1)
{
	    float Distance1,Distance2,min;
		int m,n,p,num;
		int ReceBits;

	    DistanceM[0]=Distance[0];
		DistanceM[1]=Distance[1];
		DistanceM[2]=Distance[2];
		DistanceM[3]=Distance[3];
		
		for(p=0;p<=N;p++)
		{
		 Path1[0][p]=Path[0][p];
		 Path1[1][p]=Path[1][p];
		 Path1[2][p]=Path[2][p];
		 Path1[3][p]=Path[3][p];
		}
		
		Distance1=DistanceM[0]+pow((ReceiveMes1+2),2);
		Distance2=DistanceM[2]+pow((ReceiveMes1+1),2);
		
		if (Distance1<Distance2)
		{
			
			Path[0][N+1]=0;
			Distance[0]=Distance1;
		}
		else
		{
			for(n=0;n<=N;n++)
			{
			 Path[0][n]=Path1[2][n];
			}
			Path[0][N+1]=0;
			Distance[0]=Distance2;
		}
		
		Distance1=DistanceM[0]+pow((ReceiveMes1+1),2);
		Distance2=DistanceM[2]+pow((ReceiveMes1-0),2);
		
		if (Distance1<Distance2)
		{
			for(n=0;n<=N;n++)
			{
			 Path[1][n]=Path1[0][n];
			}
			Path[1][N+1]=1;
			Distance[1]=Distance1;
		}
		else
		{
			for(n=0;n<=N;n++)
			{
			 Path[1][n]=Path1[2][n];
			}
			Path[1][N+1]=1;
			Distance[1]=Distance2;
		}
		
		Distance1=DistanceM[1]+pow((ReceiveMes1-0),2);
		Distance2=DistanceM[3]+pow((ReceiveMes1-1),2);
		
		if (Distance1<Distance2)
		{
			for(n=0;n<=N;n++)
			{
			 Path[2][n]=Path1[1][n];
			}
			Path[2][N+1]=2;
			Distance[2]=Distance1;
		}
		else
		{
			for(n=0;n<=N;n++)
			{
			 Path[2][n]=Path1[3][n];
			}
			Path[2][N+1]=2;
			Distance[2]=Distance2;
		}
		
		Distance1=DistanceM[1]+pow((ReceiveMes1-1),2);
		Distance2=DistanceM[3]+pow((ReceiveMes1-2),2);
		
		if (Distance1<Distance2)
		{
			for(n=0;n<=N;n++)
			{
			 Path[3][n]=Path1[1][n];
			}
			Path[3][N+1]=3;
			Distance[3]=Distance1;
		}
		else
		{
			Path[3][N+1]=3;
			Distance[3]=Distance2;
		}	

        //min=Distance[0];
		//num=0;
		//for(p=1;p<=3;p++)
		//{
		//if (min>Distance[p])
		//	{
		//	min=Distance[p];
		//	num=p;
		//	}
		//}
		num=1;

 if ((Path[num][0]==0) && (Path[num][1]==0))
 	ReceBits=0;
 if ((Path[num][0]==0) && (Path[num][1]==1))
 	ReceBits=1;
 if ((Path[num][0]==1) && (Path[num][1]==2))
 	ReceBits=0;
 if ((Path[num][0]==1) && (Path[num][1]==3))
 	ReceBits=1;
 if ((Path[num][0]==2) && (Path[num][1]==0))
 	ReceBits=0;
 if ((Path[num][0]==2) && (Path[num][1]==1))
 	ReceBits=1;
 if ((Path[num][0]==3) && (Path[num][1]==2))
 	ReceBits=0;
 if ((Path[num][0]==3) && (Path[num][1]==3))
 	ReceBits=1;

        for(m=0;m<=N;m++)
		{
		 Path[0][m]=Path[0][m+1];
		 Path[1][m]=Path[1][m+1];
		 Path[2][m]=Path[2][m+1];
		 Path[3][m]=Path[3][m+1];
		 //printf("%d",Path[1][m]);
		}
	//printf("%f %f %f %f\n",Distance[0],Distance[1],Distance[2],Distance[3]);	

return ReceBits;
}


