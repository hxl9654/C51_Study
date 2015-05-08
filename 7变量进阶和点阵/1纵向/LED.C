#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;
uint8 i=0,p=0,t;
uint8 pic[]={	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
				0xFF,0xC3,0xE7,0xE7,0xE7,0xE7,0xC3,0xFF,
				0xDB,0x81,0x00,0x00,0x00,0x81,0xC3,0xE7,
				0xFF,0x99,0x99,0x99,0x99,0x81,0xC3,0xFF,
				0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			};
void main()
{
	ADDR3=0;
	ENLED=0;
	TMOD=0X01;
	TH0=0XFC;
	TL0=0X67;
	EA=1;
	ET0=1;
	TR0=1;
	while(1)
	{
		if(t>=200)
			{
				t=0;
				p++;
				if(p>=32)p=0;
			}	
	}
}
void refresh() interrupt 1
{
	TH0=0XFC;
	TL0=0X67;
	i++;
	if(i>=8)i=0;
	t++;
	switch(i)
	{
		case 0:ADDR0=0;ADDR1=0;ADDR2=0;P0=pic[p+i];break;
		case 1:ADDR0=1;ADDR1=0;ADDR2=0;P0=pic[p+i];break;
		case 2:ADDR0=0;ADDR1=1;ADDR2=0;P0=pic[p+i];break;
		case 3:ADDR0=1;ADDR1=1;ADDR2=0;P0=pic[p+i];break;
		case 4:ADDR0=0;ADDR1=0;ADDR2=1;P0=pic[p+i];break;
		case 5:ADDR0=1;ADDR1=0;ADDR2=1;P0=pic[p+i];break;
		case 6:ADDR0=0;ADDR1=1;ADDR2=1;P0=pic[p+i];break;
		case 7:ADDR0=1;ADDR1=1;ADDR2=1;P0=pic[p+i];break;
	}
}