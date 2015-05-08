#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit Out=P0^0;
bit TempReadFlag=1;
bit ToHot=0,Run=0;
extern void UART_Driver();
extern void UART_RxMonitor(uint8 ms);
extern void UART_SendString(uint8 *dat,uint8 len);
extern void UART_Conf(uint16 baud);
extern bit Get18B20Temp(int *temp);
extern bit Start18B20();
uint8 T0H,T0L;
void HotClose()
{
	UART_SendString("Hot!Closed!",11);
	ToHot=1;
	Out=0;
	Run=0;
}
void UART_Action(uint8 *dat,uint8 len)
{
	if((*dat=='o')||(*dat=='O'))
		if((*(dat+1)=='n')||(*(dat+1)=='N'))
		{
			if(ToHot==0)
			{
				Out=1;
				Run=1;
				UART_SendString("Opened!",7);
			}
			else UART_SendString("ToHot!",6);
		}
		else if(((*(dat+1)=='f')||(*(dat+1)=='F'))&&((*(dat+2)=='f')||(*(dat+2)=='F')))
		{
			Out=0;
			Run=0;
			UART_SendString("Closed!",7);
		}
	UART_SendString("\r\n",2);		
}
void Timer0_Init(uint8 ms)
{
	uint16 temp;
	temp=65536-ms*110592/120;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)temp;
	TH0=T0H;
	TL0=T0L;
	TMOD&=0XF0;
	TMOD|=0X01;
	ET0=1;
	TR0=1;
}
void main()
{
	int temp;
	Out=0;
	Timer0_Init(1);
	UART_Conf(9600);
	Start18B20();
	while(1)
	{
		UART_Driver();
		if(TempReadFlag)
		{
			TempReadFlag=0;
			Get18B20Temp(&temp);
			temp>>=4;
			if(temp>=50&&Run)HotClose();
			else if(temp<50)ToHot=0;
			Start18B20();
		}	 	
	}
}

void it0() interrupt 1
{
	static uint16 ms1000=0;
	TH0=T0H;
	TL0=T0L;
	UART_RxMonitor(1);
	ms1000++;
	if(ms1000>=1000)
	{
		ms1000=0;
		TempReadFlag=1;				
	}
	
}