#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
extern void PCF8591_DA(uint8 dat);	//启动DA输出（dat：要输出的AD值）。
extern void KeyScan();	//在定时器中断中调用，读取按键信息
extern void KeyDriver();	//在主循环中调用，触发按键操作

unsigned char code SinWave[] = {  //正弦波波表
    127, 152, 176, 198, 217, 233, 245, 252,
    255, 252, 245, 233, 217, 198, 176, 152,
    127, 102,  78,  56,  37,  21,   9,   2,
      0,   2,   9,  21,  37,  56,  78, 102,
};
unsigned char code TriWave[] = {  //三角波波表
      0,  16,  32,  48,  64,  80,  96, 112,
    128, 144, 160, 176, 192, 208, 224, 240,
    255, 240, 224, 208, 192, 176, 160, 144,
    128, 112,  96,  80,  64,  48,  32,  16,
};
unsigned char code SawWave[] = {  //锯齿波表
      0,   8,  16,  24,  32,  40,  48,  56,
     64,  72,  80,  88,  96, 104, 112, 120,
    128, 136, 144, 152, 160, 168, 176, 184,
    192, 200, 208, 216, 224, 232, 240, 248,
};
uint8 T0H,T0L,T1H,T1L;
uint8 f=10,Index=1;
void F_Set_Timer1(uint8 f);

void KeyAction(uint8 keyc,uint8 statu)
{
	if(statu==1)return ;
	if(keyc==0x26)//up
	{
		if(f<27)f++;
		F_Set_Timer1(f);	
	}
    else if(keyc==0x28)//down
	{
		if(f>1)f--;
		F_Set_Timer1(f);
	}
	else if(keyc==0x31)
	{
		Index=1;
	}
	else if(keyc==0x32)
	{
		Index=2;
	}
	else if(keyc==0x33)
	{
		Index=3;
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
void F_Set_Timer1(uint8 f)
{
	uint16 temp;
	temp=65536-11059200/12/f/32;
	T1H=(uint8)(temp>>8);
	T1L=(uint8)temp;
	TH1=T1H;
	TL1=T1L;
	TMOD&=0X0F;
	TMOD|=0X10;
	ET1=1;
	PT1=1;
	TR1=1;
}
void main()
{
	EA=1;
	Timer0_Init(1);
	F_Set_Timer1(10);
	while(1)
	{		
		KeyDriver();		
	}
}
void Interrupt_Timer1() interrupt 3
{
	static uint8 i=0;
	TH1=T1H;
	TL1=T1L;
	i++;
	if(i==32)i=0;
	switch(Index)
	{
		case 1:PCF8591_DA(SinWave[i]);break;
		case 2:PCF8591_DA(TriWave[i]);break;
		case 3:PCF8591_DA(SawWave[i]);break;
	}
}
void Interrupt_Timer0() interrupt 1
{
	TH0=T0H;
	TL0=T0L;
	KeyScan();
}