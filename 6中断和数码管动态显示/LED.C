#include<reg51.h>
typedef unsigned char uint8;
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
uint16 t=0,t1=0;
uint8 LED[7]={0},i=0;
void main()
{
	TH0=0xFC;
	TL0=0x67;
	TMOD=0X01;
	EA=1;
	ET0=1;
	TR0=1;
	ADDR3=1;
	ENLED=0;
	while(1)
	if(t1>=100)
	{
		t1=0;
		t++;
		LED[0]=t%10;
		LED[1]=t/10%10;
		LED[2]=t/100%10;
		LED[3]=t/1000%10;
		LED[4]=t/10000%10;
		LED[5]=t/100000%10;
		LED[6]=(uint8)rand();
	}

}
void t0i() interrupt 1
{
	P0=0xFF;
	TH0=0xFC;
	TL0=0x67;
	switch(i)
	{
		 case 0 : ADDR0=0;ADDR1=0;ADDR2=0;P0=LedChar[LED[0]];;break;
		 case 1 : ADDR0=1;ADDR1=0;ADDR2=0;P0=(LedChar[LED[1]]&0x7F);break;
		 case 2 : ADDR0=0;ADDR1=1;ADDR2=0;P0=LedChar[LED[2]];break;
		 case 3 : ADDR0=1;ADDR1=1;ADDR2=0;P0=LedChar[LED[3]];break;
		 case 4 : ADDR0=0;ADDR1=0;ADDR2=1;P0=LedChar[LED[4]];break;
		 case 5 : ADDR0=1;ADDR1=0;ADDR2=1;P0=LedChar[LED[5]];break;
		 case 6 : ADDR0=0;ADDR1=1;ADDR2=1;P0=LED[6];break;
	}
	i++;
	t1++;
	if(i>=7)i=0;
}