#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
uint8 light=0;
bit t1s=1;
uint8 ledbuff[8];
uint8 time=10;
code uint8 ledchar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
void change()
{
	time--;
	if(time==0)
	{
		if(light==0){light=2;time=11;}
		else if(light==1){light=0;time=8;}
		else if(light==2){light=1;time=2;}
	}
}
void main()
{
	TMOD&=0XF0;
	TMOD|=0X01;
	EA=1;
	ET0=1;	
	TH0=0XFC;
	TL0=0X67;
	TR0=1;
	P1&=0xef;
	while(1)
	{
		if(t1s){t1s=0;change();}
	}	
}
void t0() interrupt 1
{
	static uint16 i1s=0;
	static uint8 i=0;
	TH0=0XFC;
	TL0=0X67;
	i1s++;
	if(i1s>=1000)
	{
		i1s-=1000;
		t1s=1;
	}
	ledbuff[0]=ledchar[time%10];
	ledbuff[1]=ledchar[time/10];
	if(light==0)ledbuff[6]=0x3f;
	else if(light==1)ledbuff[6]=0xE7;
	else if(light==2)ledbuff[6]=0xfc;
	P0=0XFF;
	P1=(P1&0XF8)|i;
	P0=ledbuff[i];
	
	if(i==0)i=1;
	else if(i==1)i=6;
	else if(i==6)i=0;
}
