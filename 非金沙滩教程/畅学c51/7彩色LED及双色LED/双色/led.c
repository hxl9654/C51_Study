#include<reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit P1_0=P1^0;
sbit P1_1=P1^1;

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 22;
	j = 3;
	k = 227;
	while (--j);
/*	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);	*/
}
 
void main()
{
	while(1)
	{
		P1_0=1;
		P1_1=0;
		Delay500ms();
		P1_0=0;
		P1_1=1;
		Delay500ms();
	}
}