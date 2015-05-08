#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;
uint8 code LedChar[16]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
uint8 code LedLocal[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
uint8 LedBuff[8]={0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0};
uint32 num=0;
uint8 T0H,T0L;
bit Flag_100ms=0;

extern void _74HC595_WriteData(uint8 dat);	//向74HC595写一个字节的数据（dat：数据）（所有数据发送完后，请调用_74HC595_Change();）。
extern void _74HC595_Change();				//向74HC595发送更新输出指令

void Timer0_Init(uint8 ms)
{
	uint16 temp;
	temp=65536-ms*110592/120;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)temp;
	TH0=T0H;
	TL0=T0L;
	TMOD=0X01;
	ET0=1;
	EA=1;
	TR0=1;
}
void RefreshLed()
{
	static uint8 i=0;
	_74HC595_WriteData(LedLocal[i]);
	_74HC595_WriteData(LedBuff[i]);
	_74HC595_Change();
	i++;
	if(i>=8)i=0;
}
void main()
{
	uint8 i,temp1;
	uint32 temp;
	Timer0_Init(1);
	while(1)
	{
		if(Flag_100ms)
			{
				num++;
				Flag_100ms=0;
				temp=num;
				for(i=0;i<8;i++)
				{					
					temp1=temp%10;
					LedBuff[i]=LedChar[temp1];					
					if(temp1)break;
					temp/=10;
				}
				LedBuff[1]&=0x7F;				
			}
		RefreshLed();
	}		
}
void Interrupt_Timer0() interrupt 1
{
	static uint8 ms100=0;
	TH0=T0H;
	TL0=T0L;
	ms100++;
	if(ms100>=100)
	{
		ms100-=100;
		Flag_100ms=1;
	}
}