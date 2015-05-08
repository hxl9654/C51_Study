#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

extern void DS1302_Init();
extern uint8 DS1302_SingleRead(uint8 addr);
extern void DS1302_SingleWrite(uint8 addr,uint8 dat);
extern void DS1302_BrustWrite(uint8 *dat);
extern void DS1302_BrustRead(uint8 *dat);

extern void LCD1602_Show(uint8 x,uint8 y,uint8 *dat,uint8 len);
extern void LCD1602_Init();
bit Flag_200ms=0;
uint8 T0H,T0L;

void Timer0_Conf(uint8 ms)
{
	uint16 temp;
	temp=65536-ms*110592/120;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)temp;
	TH0=T0H;
	TL0=T0L;
	ET0=1;
	EA=1;
	TR0=1;
}

void main()
{
	uint8 dat[8];
	uint8 dat[8]={0x00,0x15,0x91,0x09,0x12,0x02,0x14,0x00};
	uint8 temp[16];
	uint8 backup=0xAA;
	LCD1602_Init();
	Timer0_Conf(1);
	DS1302_Init(dat);
	while(1)
	{
		if(Flag_200ms)
		{
			Flag_200ms=0;
			DS1302_BrustRead(dat);
			if(dat[0]==backup)continue;
			backup=dat[0];
			temp[0]='2';
			temp[1]='0';
			temp[2]=(dat[6]>>4)+'0';
			temp[3]=(dat[6]&0x0F)+'0';
			temp[4]='-';
			temp[5]=(dat[4]>>4)+'0';
			temp[6]=(dat[4]&0x0F)+'0';
			temp[7]='-';
			temp[8]=(dat[3]>>4)+'0';
			temp[9]=(dat[3]&0x0F)+'0';
			temp[10]=' ';
			temp[11]='w';
			temp[12]='e';
			temp[13]='e';
			temp[14]='k';
			temp[15]=dat[5]+'0';
			LCD1602_Show(0,0,temp,16);
			temp[0]=' ';
			temp[1]=' ';
			temp[2]=' ';
			temp[3]=' ';
			temp[4]=((dat[2]>>4)&0x07)+'0';
			temp[5]=(dat[2]&0x0F)+'0';
			temp[6]=':';
			temp[7]=(dat[1]>>4)+'0';
			temp[8]=(dat[1]&0x0F)+'0';
			temp[9]=':';
			temp[10]=((dat[0]>>4)&0x07)+'0';
			temp[11]=(dat[0]&0x0F)+'0';
			temp[12]=' ';
			temp[13]=' ';
			temp[14]=' ';
			temp[15]=' ';
			LCD1602_Show(0,1,temp,16);
		}
	}
}
void it0() interrupt 1
{
	static uint8 ms200=0;
	TH0=T0H;
	TL0=T0L;
	ms200++;
	if(ms200>=200)
		{
			ms200=0;
			Flag_200ms=1;
		}
}