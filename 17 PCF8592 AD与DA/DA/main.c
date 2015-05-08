#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
extern void PCF8591_DA(uint8 dat);	//启动DA输出（dat：要输出的AD值）。
extern void KeyScan();	//在定时器中断中调用，读取按键信息
extern void KeyDriver();	//在主循环中调用，触发按键操作

uint16 dat=300;
uint8 T0H,T0L;

void KeyAction(uint8 keyc,uint8 statu)
{
	if(statu==1)return ;
	if(keyc==0x26)//up
	{
		if(dat<=490)dat+=10;
		else dat=499;	
	}
    else if(keyc==0x28)//down
	{
		if(dat>=10)dat-=10;
		else dat=0;
	}
}
void Timer0_Init(uint8 ms)
{
	uint16 temp;
	temp=65536-ms*110592/120;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)temp;
	TH0=T0H;
	TL0=T0L;
	TMOD&=0XF0;
	TMOD|=0X01;
	ET0=1;
	TR0=1;
}
void main()
{
	uint16 temp;
	EA=1;
	Timer0_Init(1);
	while(1)
	{
		temp=64*dat/125;
		PCF8591_DA(temp);
		KeyDriver();		
	}
}
void Interrupt_Timer0() interrupt 1
{
	TH0=T0H;
	TL0=T0L;
	KeyScan();
}