#include<reg51.h>
sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;
sbit LED=P0^0;
int main()
{
	ADDR0=0;ADDR1=1;ADDR2=1;
	ADDR3=1;ENLED=0;
	LED=0;
	while(1);
	return 0;
}
