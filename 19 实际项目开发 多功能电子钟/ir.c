#include"config.h"

uint8 IRBuff[4];
bit Flag_IR=0;

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