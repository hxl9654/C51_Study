#include<reg51.h>
#include<LED.h>
void Delay300ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 13;
	j = 156;
	k = 83;
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
	uint8 i;
	while(1)
	{
		for(i=0;i<32;i++)
			{
				P0=LedMap[i][0];
				P1=LedMap[i][1];
				P2=LedMap[i][2];
				P3=LedMap[i][3];
				Delay300ms();
			}
	}
}
