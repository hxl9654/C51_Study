#include<reg51.h>
#define BUFFMAX 64
#define XTAL 11059200
typedef unsigned char uint8;
typedef unsigned int uint16;
extern void UART_Action(uint8 *dat,uint8 len);
uint8 pdata UART_Buff[BUFFMAX];
uint8 UART_BuffIndex=0;
bit UART_SendFlag,UART_ResiveFlag,UART_ResiveStringEndFlag,UART_ResiveStringFlag;
void UART_Conf(uint16 baud)
{
	TL1=TH1=256-XTAL/12/32/baud;
	EA=1;
	ES=1;
	TMOD&=0X0F;
	TMOD|=0X20;
	SCON=0X50;
	TR1=1;
}
void UART_SendString(uint8 *dat,uint8 len)
{
	while(len)
	{
		len--;
		SBUF=*dat;
		dat++;
		while(!UART_SendFlag);
		UART_SendFlag=0;
	}
}
uint8 UART_Read(uint8 *to,uint8 len)
{
	uint8 i;
	if(UART_BuffIndex<len)len=UART_BuffIndex;
	for(i=0;i<len;i++)
		{
			*to=UART_Buff[i];
			to++;
		}
	UART_BuffIndex=0;
	return len;	
}
void UART_Driver()//在主循环中调用
{
	uint8 pdata dat[64];
	uint8 len;
	if(UART_ResiveStringEndFlag)
		{
			UART_ResiveStringEndFlag=0;
			len=UART_Read(dat,64);
			UART_Action(dat,len);
		}	
}
void UART_RxMonitor(uint8 ms)//在定时器中调用
{
	static uint8 ms30=0,UART_BuffIndex_Backup;
	ms30+=ms;
	if(!UART_ResiveStringFlag)return ;
	if(UART_BuffIndex_Backup!=UART_BuffIndex)
	{
		UART_BuffIndex_Backup=UART_BuffIndex;
		ms30=0;
	}
	if(ms30>30)
		{
			ms30=0;
			UART_ResiveStringEndFlag=1;
			UART_ResiveStringFlag=0;
		}

}
void interrupt_UART() interrupt 4
{
	if(TI)
	{
		TI=0;
		UART_SendFlag=1;
	}
	if(RI)
	{
		RI=0;
		UART_ResiveFlag=1;
		UART_Buff[UART_BuffIndex]=SBUF;
		UART_ResiveStringFlag=1;
		UART_BuffIndex++;
	}
}