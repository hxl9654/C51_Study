#include<reg51.h>
sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;
sbit LED=P0^0;
typedef unsigned int uint16;
int main()
{
	uint16 i;
	ADDR0=0;ADDR1=1;ADDR2=1;
	ENLED=0;ADDR3=1;
	while(1)
	{
		for(i=0;i<10000;i++);
		LED=0;
		for(i=0;i<10000;i++);
		LED=1;
	}
	return 0;
}