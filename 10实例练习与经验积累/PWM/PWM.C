#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;
sbit PWM=P0^0;
uint8 T1H,T1L,T0HH,T0HL,T0LH,T0LL;
void timer0init(uint8 f,uint16 rate)
{
	uint32 temp=0,temp1=0;
	temp=11059200/12/f;
	temp1=temp*rate/100;
	temp=temp-temp1;
	temp=65536-temp;
	temp1=65536-temp1;
	T0HH=(uint8)(temp>>8);
	T0LH=(uint8)temp;
	T0HL=(uint8)(temp1>>8);
	T0LL=(uint8)temp1;
	TH0=T0HH;
	TL0=T0LH;
	TR0=1;	
}
void timer1init(uint16 ms)
{
	uint32 temp=0;
	temp=ms*110592/120;
	temp=65536-temp+12;
	T1H=(uint8)(temp>>8);
	T1L=(uint8)temp;
	TH1=T1H;
	TL1=T1L;
	TR1=1;
}
void main()
{
	//uint16 i;
	TMOD=0X11;
	PWM=0;
	P1=0XEE;
	ET0=1;
	ET1=1;
	EA=1;
	timer1init(50);
	while(1);
	/*{
		timer0init(100,90);
		for(i=0;i<10000;i++);
		timer0init(100,60);
		for(i=0;i<10000;i++);
		timer0init(100,30);
		for(i=0;i<10000;i++);
		timer0init(100,0);
		for(i=0;i<10000;i++);
	}  */
}
void t0() interrupt 1
{
	if(PWM==1)
	{
		PWM=0;
		TH0=T0HL;
		TL0=T0LL;
	}
	else 
	{
		PWM=1;
		TH0=T0HH;
		TL0=T0LH;
	}
}
void t1() interrupt 3
{
	static uint8 index=0;
	static bit dir=1;
	code uint8 ratec[13]={5,18,30,41,51,60,68,75,81,86,90,93,95};
	TH1=T1H;
	TL1=T1L;
	if(dir==1)index++;
	else index--;
	if(index==1&&dir==0)dir=1;
	else if(index==13&&dir==1)dir=0;
	timer0init(500,ratec[index-1]);
}