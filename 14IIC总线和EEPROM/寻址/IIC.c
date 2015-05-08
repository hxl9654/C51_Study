#include<reg51.h>
#include<intrins.h>
#define iicdelay() {nop_();nop_();nop_();nop_();}
typedef unsigned char uint8;
typedef unsigned int uint16;
extern void lcdinit();
extern void lcdshow(uint8 x,uint8 y,uint8 *str,uint8 n);
sbit SDA=P3^6;
sbit SCL=P3^7;

void iicstart()
{
	SCL=0;
	iicdelay();
	SDA=1;
	iicdelay();
	SCL=1;
	iicdelay();
	SDA=0;
	iicdelay();
	SCL=0;	
}
void iicstop()
{
	SCL=0;
	iicdelay();
	SDA=0;
	iicdelay();
	SCL=1;
	iicdelay();
	SDA=1;
}
bit iicwrite(uint8 d)
{
	uint8 mask=0x80,i;
	bit a;
	for(i=0;i<8;i++)
		{
			if(d&mask)SDA=1;
			else SDA=0;
			iicdelay();
			SCL=1;
			mask>>=1;
			SCL=0;
		}	
	SDA=1;
	iicdelay();
	SCL=1;
	a=SDA;
	SCL=0;
	return a;
}
void main()
{
	bit a;
	lcdinit();

	iicstart();
	a=iicwrite(0x50<<1);
	iicstop();

	lcdinit();
	lcdshow(0,0,(a==0?"y":"n"),1);
	
	
	iicstart();
	a=iicwrite(0x20<<1);
	iicstop();
	lcdshow(0,1,(a==0?"y":"n"),1);
	
	while(1);	
}