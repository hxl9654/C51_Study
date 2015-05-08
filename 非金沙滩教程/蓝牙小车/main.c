#include<reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
uint8 dat=128;
sbit l=P1^0;
sbit r=P1^1;
void UART_Conf(uint16 baud) //UART设置函数（buad：欲设置的波特率）
{
	TL1=TH1=256-11059200/12/32/baud;
	EA=1;
	ES=1;
	TMOD&=0X0F;
	TMOD|=0X20;
	SCON=0X50;
	TR1=1;
}
int main()
{
	TH0=0XFC;
	TL0=0X66;
	TMOD=0X01;
	TR0=1;
	ET0=1;
	UART_Conf(9600);	
	while(1);
}
void Interrupt_Timer0() interrupt 1
{
	static uint8 i=0;
	uint8 j;
	TH0=0XFC;
	TL0=0X66;
	if(i%2)l=0;
	else l=1;
	j=255/(255-dat);
	if(i%j)r=0;
	else r=1;
	i++;
}
void interrupt_UART() interrupt 4   //串口中断函数
{
	if(TI)
	{
		TI=0;
	}
	if(RI)
	{
		RI=0;
		dat=SBUF;
		SBUF=0XF1;
	}
}