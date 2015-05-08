#include<reg52.h>
sbit TE1=P0^0;
sbit TE2=P1^0;
sbit TE3=P2^0;
sbit TE4=P3^0;
sbit O1=P3^4;
sbit O2=P3^5;
sbit O3=P3^6;
sbit O4=P3^7;

void main()
{
	TE1=0;TE2=0;TE3=0;TE4=0;
	while(1)
	{
		if(TE1)O1=0;
		if(TE2)O2=0;
		if(TE3)O3=0;
		if(TE4)O4=0;	
	}
}													  