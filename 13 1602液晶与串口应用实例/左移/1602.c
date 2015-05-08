#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit RS=P1^0;
sbit RW=P1^1;
sbit E=P1^5;
uint8 string1[]="Hello World";
uint8 string2[]="   By HXL  ";
void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


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
void main()
{
	uint8 idata str1[100],str2[100];
	uint8 i,index=0;
	lcdinit();
	lcdinit();
	for(i=0;i<16;i++)
	{
		str1[i]=' ';
		str2[i]=' ';
		str1[16+sizeof(string1)+i]=' ';
		str2[16+sizeof(string2)+i]=' ';
	}
	for(i=0;i<sizeof(string1);i++)
		{
			str1[16+i]=string1[i];
			str2[16+i]=string2[i];
		}
	i--;
	str1[16+i]=' ';
	str2[16+i]=' ';
	while(1)
	{
	lcdshow(0,0,str1+index,16);
	lcdshow(0,1,str2+index,16);
	Delay100ms();
	index++;
	if(index>=(15+sizeof(string1)))index=0;	
	}
}