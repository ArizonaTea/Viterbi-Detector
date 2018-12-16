/*
Viterbi Detector
Finite_State_Machine -> AWGN_Channel -> Vitervi_Detector
Author: Tao Liu
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int N,M;

#define N 5
#define M 10000000
#define P 4
#define Q 4

float Distance[P];
int Path[P][N+2];

main() {
//-------------------------------------Initialization
int NN[2][P];
int OO[2][Q];
int i,j,k,l,m,o,p,q,r;
int MesKit[N+1],Stat[N+1],Output[N+1];
float gen_rand_uniform();    //function for generating uniform [0,1) random numbers
float gaussrand();   //fuction for generating AWGN noise
void Viterbi_Detector_Initial(float Receive1[N+1],int NN[2][P],int OO[2][P]);	   //function for initialize the viterbi detector
int Viterbi_Detector(float ReceiveMes1,int NN[2][P],int OO[2][P]);	//function for viterbi detector
float pi,sigma,E0,SNR;
int b;
pi=3.141592654;
E0=12/8;
SNR=2;
sigma=sqrt(2.0*E0/pow(10,(SNR/10)));
printf("SNR=%f	Sigma=%f\n",SNR,sigma);

NN[0][0]=0;
NN[0][1]=2;
NN[0][2]=0;
NN[0][3]=2;
NN[1][0]=1;
NN[1][1]=3;
NN[1][2]=1;
NN[1][3]=3;

OO[0][0]=-2;
OO[0][1]=0;
OO[0][2]=-1;
OO[0][3]=1;
OO[1][0]=-1;
OO[1][1]=1;
OO[1][2]=0;
OO[1][3]=2;

//--------------------------------Filled_the_Checking_Block_for_Viterbi_Detector(First N Bits)

for(i=1;i<=N;i++)
{
 //generating random bits 0, 1 
 b = (gen_rand_uniform()>0.5)?1:0;
 //storing the bits, reordering the bits in the memory 
 MesKit[i]=b;
}
 //Finiate State Machine 
Stat[1]=0;

for(j=1;j<=N;j++)
{
 for(m=0;m<P;m++)
 	{
 	if ((MesKit[j]==0) && (Stat[j]==m)) 
		Stat[j+1]=NN[0][m];
	if ((MesKit[j]==1) && (Stat[j]==m)) 
		Stat[j+1]=NN[1][m];
 	}
}

for(j=1;j<=N;j++)
{
	for(m=0;m<Q;m++)
	{
	if ((MesKit[j]==0) && (Stat[j]==m)) 
		Output[j]=OO[0][m];
	if ((MesKit[j]==1) && (Stat[j]==m)) 
		Output[j]=OO[1][m];
 	}
}

//AWGN Channel
float noise;
float Receive[N+1];
for(l=1;l<=N;l++)
{
 noise=gaussrand();
 Receive[l]=Output[l]+sigma*noise;
 //Receive[l]=Output[l];
}

//Viterbi Detector

for(i=0;i<P;i++)
{
Path[P][0]=P;
Distance[P]=0;
}

Viterbi_Detector_Initial(Receive,NN,OO);

//-----------------------------------The_Main_Information_Sequence_Decoded_By_Viterbi_Detector(The M Bits)

int Message,Message1,Lmessage,State,Lstate,Output1,RecBit,Error;
float ReceiveMes,BER;
Error=0;

for(r=N;r<=M+N;r++)
{
    //Generate information bit
    Message=(gen_rand_uniform()>0.5)?1:0;
	//printf("%d\n",Message);
    //Finite State Machine
    if (r==N)
	{
	Lmessage=MesKit[N];
	Lstate=Stat[N];
    }

	for(m=0;m<P;m++)
		{
		if ((Lmessage==0) && (Lstate==m)) 
			State=NN[0][m];
		if ((Lmessage==1) && (Lstate==m)) 
			State=NN[1][m];
		}
	
	for(m=0;m<Q;m++)
		{
		if ((Message==0) && (State==m)) 
			Output1=OO[0][m];
		if ((Message==1) && (State==m)) 
			Output1=OO[1][m];
		}

//AWGN_Channel		
ReceiveMes=0;
noise=gaussrand();
ReceiveMes=Output1+sigma*noise;
//printf("%f\n",ReceiveMes);
//ReceiveMes=Output1;


//Viterbi Decoder
RecBit=Viterbi_Detector(ReceiveMes,NN,OO);

//for(q=1;q<=N;q++)
//{
//Receive[q]=Receive[q+1];
//}
//Receive[N]=ReceiveMes;


Message1=MesKit[1];
for(q=1;q<N;q++)
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

//Calcuate of BER	
BER=Error/((float) (M));
printf("BER=%f\n",BER);
printf("M=%d\n",M);
printf("Error=%d\n",Error);

//for(m=0;m<=N;m++)
	//printf("%d\n",Path[0][m]);

}

//--------------------------------------------------Function for Generating Random Bits
float gen_rand_uniform() 
{
float u1;
u1 = (double)rand()/((double)RAND_MAX + (double)1); 
return(u1);
}

//--------------------------------------------Function for Generating AWGN noise
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

//--------------------------------------------------------Function for Viterbi Detector Initialization
void Viterbi_Detector_Initial(float Receive1[N+1],int NN[2][P],int OO[2][P])
{
    //NN[0][0]=0;
	float Distance1[P],DistanceM[P],min;
	int m,n,p,q,s,mark,w;
	int SS[2*P],MM[2*P];
	int Path1[P][N+2];
	//static int Pathfinal1[N+1];	
	
	for(m=1;m<=N;m++)
	{
	for(n=0;n<P;n++)
		DistanceM[n]=Distance[n];
	
	for(n=0;n<P;n++)
	{
	for(p=0;p<=m-1;p++)
		Path1[n][p]=Path[n][p];
	}

   for(w=0;w<P;w++)
   { 
   s=0;
   for(n=0;n<P;n++)
   {
   for(p=0;p<=1;p++)
   	{
   	if( NN[p][n] == w)
   	 {SS[s]=n;
      MM[s]=p;
   	  s++;}
	  //printf("%d",NN[p][n]);
	  }
   }
//printf("%d",s);
   for(q=0;q<s;q++)
	Distance1[q]=DistanceM[SS[q]]+pow((Receive1[m]-(OO[MM[q]][SS[q]])),2);

    min=Distance1[0];
    mark=0;

   for(q=1;q<s;q++)
   	if(Distance1[q]<min)
   	{min=Distance1[q];
     mark=q;}
   	
   for(n=0;n<=m-1;n++)
		{
		 Path[w][n]=Path1[SS[mark]][n];
		 //printf("%d\n",Path[w][n]);
		}
   Path[w][m]=w;
   Distance[w]=Distance1[mark];
   
}

}

}

//-----------------------------------------------------------Function for Viterbi Detector

int Viterbi_Detector(float ReceiveMes1,int NN[2][P],int OO[2][P])
{
	    float mini;
		int ReceBits;
		float Distance1[P],DistanceM[P],min;
	    int m,n,p,num,q,mark,S,w;
	    int SS[2*P],MM[2*P];
		int Path1[P][N+2];
		//NN[0][0]=0;
		for(n=0;n<P;n++)
				DistanceM[n]=Distance[n];
			
		for(n=0;n<P;n++)
			{
			for(p=0;p<=N;p++)
				Path1[n][p]=Path[n][p];
			}
		
		for(w=0;w<P;w++)
		  { 
		   S=0;
		   for(n=0;n<P;n++)
		   	for(p=0;p<=1;p++)
				 if (NN[p][n]==w)
			{
			SS[S]=n;
			MM[S]=p;
			S++;
			}
		//printf("%d",NN[p][n]);}  
		//printf("s=%d\n",S);
		for(q=0;q<S;q++)
		Distance1[q]=DistanceM[SS[q]]+pow((ReceiveMes1-(OO[MM[q]][SS[q]])),2);
		
		min=Distance1[0];
		mark=0;
		
		for(q=1;q<S;q++)
			if(Distance1[q]<min)
			 {
			 min=Distance1[q];
			 mark=q;
			 }
		//printf("mark=%d\n",mark);
		   for(n=0;n<=N;n++)
				{
				 Path[w][n]=Path1[SS[mark]][n];
				 //printf("%d\n",Path[w][n]);
				}
		   Path[w][N+1]=w;
		   Distance[w]=Distance1[mark];  
		}

        mini=Distance[0];
		num=0;
		for(p=1;p<P;p++)
		{
		if (mini>Distance[p])
			{
			mini=Distance[p];
			num=p;
			}
		}

 if(NN[0][Path[num][0]]==Path[num][1])
 	ReceBits=0;
 if(NN[1][Path[num][0]]==Path[num][1])
 	ReceBits=1;

 for(m=0;m<=N;m++)
	 for(n=0;n<P;n++)
		Path[n][m]=Path[n][m+1];
     //printf("%d",Path[1][m]);
	//printf("%f %f %f %f\n",Distance[0],Distance[1],Distance[2],Distance[3]);	
for(p=0;p<=3;p++)
//printf("%d 	%f\n",p,Distance[p]);
return ReceBits;
}
