#include<reg51.h>
typedef unsigned int uint16;
sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;
unsigned char code LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
void main()
{
	uint16 c=0,n=0;
	ADDR0=0;ADDR1=0;ADDR2=0;
	ADDR3=1;ENLED=0;
	TMOD=0x01;
	P0=LedChar[n];
	while(1)
	{
		TH0=0xFC;
		TL0=0x5D;
		TR0=1;
		while(!TF0);
		TF0=0;
		c++;
		if(c>=1000)
		{
			n++;
			if(n>=16)n=0;
			P0=LedChar[n];
			c=0;
		}
	}
}