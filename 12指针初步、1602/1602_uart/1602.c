#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit RS=P1^0;
sbit RW=P1^1;
sbit E=P1^5;
uint8 string[20]=0;
uint8 n;
bit rend=0;
void lcdwait()
{
	uint8 sta;
	RS=0;
	RW=1;
	do{
	E=1;
	sta=P0;
	E=0;
	}while(sta&0x80);
}
void lcdwritecmd(uint8 cmd)
{
	lcdwait();
	RS=0;
	RW=0;
	P0=cmd;
	E=1;
	E=0;
}
void lcdwritedata(uint8 da)
{
	lcdwait();
	RS=1;
	RW=0;
	P0=da;
	E=1;
	E=0;
}
void setlocal(uint8 x,uint8 y)
{
	uint8 addr;
	if(y==0)addr=0x00+x;
	else addr=0x40+x;
	lcdwritecmd(addr|0x80);
}
void lcdshow(uint8 x,uint8 y,uint8 *str)
{
	setlocal(x,y);
	while(*str!='\0')
	{
		lcdwritedata(*str);
		str++;
	}
}
void lcdinit()
{
	E=0;
	lcdwritecmd(0x38);
	lcdwritecmd(0x0C);
	lcdwritecmd(0x06);
	lcdwritecmd(0x01);
}
void uartinit(uint16 baud)
{
	TH1=TL1=256-11059200/12/32/baud;
	TMOD=0X20;
	SCON=0X50;
	EA=1;
	ES=1;
	TR1=1;
}
void t0init()
{
	ET0=1;
	TH0=TL0=0;
	TR0=1;
}
void main()
{
	lcdinit();
	uartinit(9600);	
	while(1)
	{
		while(!rend);
		rend=0;
		lcdwritecmd(0x01);
		lcdshow(0,0,string);
	}
}
void is() interrupt 4
{
	string[n]=SBUF;
	n++;
	t0init();
	RI=0;
}
void it0() interrupt 1
{
	ET0=0;								   l
	TH0=TL0=0;
	rend=1;
	string[n]=0;
	n=0;
}