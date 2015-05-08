#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
uint8 *p;
uint8 size,d;
bit rend=0;
void confbaud(uint16 baud)
{
	TH1=TL1=256-11059200/12/32/baud;
	TMOD=0x20;
	SCON=0X50;
	EA=1;
	ES=1;
	TR1=1;
}
void main()
{
	uint8 a1[]={0,1};
	uint8 a2[]={0,1,2,3};
	uint8 a3[]={0,1,2,3,4,5};
	confbaud(9600);
	while(1)
	{
		if(rend)
		{
			switch(d)
			{
				case 1:p=a1;size=sizeof(a1);TI=1;break;
				case 2:p=a2;size=sizeof(a2);TI=1;break;
				case 3:p=a3;size=sizeof(a3);TI=1;break;
			}
			rend=0;
		}
	}
}																				 
void si() interrupt 4
{
	if(RI)					
	{
		RI=0;
		d=SBUF;
		rend=1;
	}
	else if(TI)
	{
		TI=0;
		if(size==0)return ;
		SBUF=*p;
		p++;
		size--;
	}
}