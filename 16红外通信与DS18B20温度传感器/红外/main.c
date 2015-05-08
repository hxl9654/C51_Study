#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
uint8 code LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
uint8 LedBuff[6]={0xff,0xff,0xff,0xff,0xff,0xff};
uint8 IRBuff[4];
uint8 T0H,T0L;
bit Flag_IR=0;
void Timer0_Init(uint8 ms)
{
	uint16 temp;
	temp=65536-ms*110592/120;
	temp+=12;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)(temp);
	TH0=T0H;
	TL0=T0L;
	TMOD&=0XF0;
	TMOD|=0X01;
	ET0=1;
	PT0=1;
	TR0=1;
}
void INT0_Init()
{
	IT1=1;
	EX1=1;
}
void Timer1_Init()
{
	TH1=0;
	TL1=0;
	TMOD&=0X0F;
	TMOD|=0X10;
}
void main()
{
	EA=1;
	P1&=0XEF;
	Timer0_Init(1);
	Timer1_Init();
	INT0_Init();
	while(1)
	{
		if(Flag_IR)
		{
			Flag_IR=0;
			LedBuff[0]=LedChar[IRBuff[0]/16];
			LedBuff[1]=LedChar[IRBuff[0]%16];;
			LedBuff[4]=LedChar[IRBuff[2]/16];;
			LedBuff[5]=LedChar[IRBuff[2]%16];;
		}
	}
}
uint16 ReadLowTime()
{
	TH1=0;
	TL1=0;
	TR1=1;
	while(!INT1)
	{
		if(TH1>40)break;
	}
	TR1=0;
	return TH1*256+TL1;
}
uint16 ReadHighTime()
{
	TH1=0;
	TL1=0;
	TR1=1;
	while(INT1)
	{
		if(TH1>40)break;
	}
	TR1=0;
	return TH1*256+TL1;
}
void Intrrrupt_INT1() interrupt 2
{
	uint16 time;
	uint8 i,j,dat;
	time=ReadLowTime();
	if(time<7500||time>9000)return ;
	time=ReadHighTime();
	if(time<3500||time>5000)return ;
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{
			time=ReadLowTime();
			if(time<300||time>700)return ;
			time=ReadHighTime();
			if(time>300&&time<700)
				dat>>=1;	
			else if(time>1300&&time<1800)
				{
					dat>>=1;
					dat|=0x80;
				}
			else return ;
		}
		IRBuff[i]=dat;	
	}
	Flag_IR=1;	
}
void Interrupt_Timer0() interrupt 1
{
	static uint8 i=0;
	TH0=T0H;
	TL0=T0L;
	P1&=0XF8;
	P1|=i;
	P0=LedBuff[i];
	i++;
	if(i>5)i=0;	
}