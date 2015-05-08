#include <reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit BUZZ=P1^6;
uint8 TH;
uint8 TL;
void buzzstop()
{
	EA=0;
	BUZZ=1;
}
void buzzer(uint16 x)
{
	uint16 t=0;
	t=65536-11059200/24/x;
	TL=t&0X00FF;
	TH=t>>8;
	TH0=0XFF;
	TL0=0XFE;
	TR0=1;
	EA=1;
}
void main()
{
	uint16 i;
	EA=1;
	TMOD=0X01;
	ET0=1;
	while(1)
	{
		buzzer(4000);
		for(i=0;i<30000;i++);
		buzzstop();
		for(i=0;i<30000;i++);
		buzzer(1000);
		for(i=0;i<30000;i++);
		buzzstop();
		for(i=0;i<30000;i++);
	}
}
void buzzerbeep() interrupt 1
{
	TH0=TH;
	TL0=TL;
	BUZZ=~BUZZ;
}