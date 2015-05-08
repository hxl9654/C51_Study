#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
uint16 num=0;
uint8 T0H,T0L;
unsigned char code LedChar[] = {  //数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};
uint8 ledbuff[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
void confbaud(uint16 baud)
{
	TH1=TL1=256-11059200/12/32/baud;
	TMOD&=0X0F;
	TMOD|=0X20;
	SCON=0X50;
	EA=1;
	ES=1;
	TR1=1;
}
void sett0(uint8 t)
{
	uint16 temp;
	TMOD&=0XF0;
	TMOD|=0X01;
	temp=t*110592/120;
	temp=65535-temp;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)temp;
	TH0=T0H;
	TL0=T0L;
	ET0=1;
	TR0=1;	
}
void main()
{
	uint16 num1;
	uint8 i;
	confbaud(9600);
	sett0(1);
	while(1)
	{
		num1=num;
		for(i=0;i<6;i++)
		{
			ledbuff[i]=num1%10;
			num1/=10;
		}	
	}
}
void it0() interrupt 1
{
	static uint8 i=0;
	TH0=T0H;
	TL0=T0L;
	P0=0XFF;
	P1&=0XE8;
	P1|=i;
	P0=LedChar[ledbuff[i]];
	i++;
	if(i>=6)i=0;
	
}
void is() interrupt 4
{
	if(RI)
	{
		RI=0;
		num=SBUF;
	}
}