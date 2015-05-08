#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
uint8 string1[]="Hello World";
uint8 string2[]="   By HXL  ";
extern void lcdshow(uint8 x,uint8 y,uint8 *str,uint8 n);
extern void lcdinit();
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