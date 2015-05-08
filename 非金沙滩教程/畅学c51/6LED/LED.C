#include<reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 22;
	j = 3;
	k = 227;
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
	uint8 a;
	while(1)
	{
		P1=~a;
		a>>=1;
		Delay500ms();
		if(a==0)a=0x80;
	}
}