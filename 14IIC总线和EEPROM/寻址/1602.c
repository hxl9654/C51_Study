#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit RS=P1^0;
sbit RW=P1^1;
sbit E=P1^5;

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
void lcdshow(uint8 x,uint8 y,uint8 *str,uint8 n)
{
	uint8 i=0;
	setlocal(x,y);
	while(n--)
	{
		lcdwritedata(*str++);
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
