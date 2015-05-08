#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
bit sendf=0;
extern uint8 idata d[100];
uint8 index=0;
extern uint8 T0H,T0L;
extern bit rendf;
extern bit run;
extern uint8 t30;
void uartinit(uint16 baud)
{
	EA=1;
	ES=1;
	SCON=0X50;
	TMOD&=0X0F;
	TMOD|=0X20;
	TH1=TL1=256-11059200/12/32/baud;
	TR1=1;
}

void si() interrupt 4
{
	if(TI)
		{
			TI=0;
			sendf=1;
		}
	else if(RI)
		{
			RI=0;
			d[index]=SBUF;
			index++;
			t30=0;
			run=1;
		}
}