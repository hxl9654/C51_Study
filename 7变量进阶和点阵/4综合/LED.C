#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;
uint16 t=0,ti=0;
uint8 i=0,p=10;
unsigned char code LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
uint8 code pic[10][8] = {
    {0xFF,0xC3,0xDB,0xDB,0xDB,0xDB,0xC3,0xFF},  //0
	{0xFF,0xE7,0xE3,0xE7,0xE7,0xE7,0xC3,0xFF},  //1
	{0xFF,0xC3,0xDF,0xC3,0xFB,0xC3,0xFF,0xFF},  //2
	{0xFF,0xC3,0xDF,0xC3,0xDF,0xC3,0xFF,0xFF},  //3
	{0xFF,0xEB,0xEB,0xE3,0xEF,0xEF,0xFF,0xFF},  //4
    {0xFF,0xE3,0xFB,0xE3,0xEF,0xE3,0xFF,0xFF},  //5
    {0xFF,0xE3,0xFB,0xE3,0xEB,0xE3,0xFF,0xFF},  //6
    {0xFF,0xE3,0xEF,0xEF,0xEF,0xEF,0xFF,0xFF},  //7
    {0xFF,0xE3,0xEB,0xE3,0xEB,0xE3,0xFF,0xFF},  //8
    {0xFF,0xE3,0xEB,0xE3,0xEF,0xE3,0xFF,0xFF},  //9
			};
uint8 n[7]={0};
void main()
{	
	ENLED=0;
	TMOD=0X01;
	TH0=0XFE;
	TL0=0X34;
	EA=1;
	ET0=1;
	TR0=1;
	while(1)
	{
		if(t>=200)
			{
				t=0;
				ti++;
				p=ti%10;
				n[0]=ti/10%10;
				n[1]=ti/100%10;
				n[2]=ti/1000%10;
				n[3]=ti/10000%10;
				n[4]=ti/100000%10;
				n[5]=ti/1000000%10;
				n[6]=(uint8)rand();
			}	
	}
}
void refresh() interrupt 1
{
	TH0=0XFE;
	TL0=0X34;
	i++;
	if(i>14)i=0;
	t++;
	P0=0XFF;
	switch(i)
	{
		case 0:ADDR3=0;ADDR0=0;ADDR1=0;ADDR2=0;P0=pic[p][i];break;
		case 1:ADDR3=0;ADDR0=1;ADDR1=0;ADDR2=0;P0=pic[p][i];break;
		case 2:ADDR3=0;ADDR0=0;ADDR1=1;ADDR2=0;P0=pic[p][i];break;
		case 3:ADDR3=0;ADDR0=1;ADDR1=1;ADDR2=0;P0=pic[p][i];break;
		case 4:ADDR3=0;ADDR0=0;ADDR1=0;ADDR2=1;P0=pic[p][i];break;
		case 5:ADDR3=0;ADDR0=1;ADDR1=0;ADDR2=1;P0=pic[p][i];break;
		case 6:ADDR3=0;ADDR0=0;ADDR1=1;ADDR2=1;P0=pic[p][i];break;
		case 7:ADDR3=0;ADDR0=1;ADDR1=1;ADDR2=1;P0=pic[p][i];break;
		case 8:ADDR3=1;ADDR0=0;ADDR1=0;ADDR2=0;P0=LedChar[n[i-8]];break;
		case 9:ADDR3=1;ADDR0=1;ADDR1=0;ADDR2=0;P0=LedChar[n[i-8]];break;
		case 10:ADDR3=1;ADDR0=0;ADDR1=1;ADDR2=0;P0=LedChar[n[i-8]];break;
		case 11:ADDR3=1;ADDR0=1;ADDR1=1;ADDR2=0;P0=LedChar[n[i-8]];break;
		case 12:ADDR3=1;ADDR0=0;ADDR1=0;ADDR2=1;P0=LedChar[n[i-8]];break;
		case 13:ADDR3=1;ADDR0=1;ADDR1=0;ADDR2=1;P0=LedChar[n[i-8]];break;
		case 14:ADDR3=1;ADDR0=0;ADDR1=1;ADDR2=1;P0=LedChar[n[i-8]];break;
	}
	;
}