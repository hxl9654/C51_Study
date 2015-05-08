#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
void confbaud(uint16 baud)
{
	TH1=256-11059200/12/32/baud;
	TL1=TH1;
	TMOD&=0X0F;
	TMOD|=0X20;
	EA=1;
	ES=1;
	SCON=0X50;
	TR1=1;
}
void main()
{
	confbaud(9600);
	while(1);
}
void si() interrupt 4
{
	if(RI)
	{
		RI=0;
		SBUF=SBUF+1;
	}
	else TI=0;
}