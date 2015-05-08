#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
extern void UART_Driver();
extern void UART_Conf(uint16 baud);
extern void UART_RxMonitor(uint8 ms);
extern void LCD1602_Init();
extern void EEPROM_WriteString(uint8 addr,uint8 *dat,uint8 len);
extern void EEPROM_ReadString(uint8 addr,uint8 len,uint8 *dat);
extern void LCD1602_Show(uint8 x,uint8 y,uint8 *dat,uint8 len);
extern void UART_SendString(uint8 *dat,uint8 len);
extern void LCD1602_AreaClear(uint8 x,uint8 y,uint8 len);
code uint8 UART_Header[]="show";
code uint8 spaces[]="                   ";
uint8 T0H,T0L;
void UART_Action(uint8 *dat,uint8 len)
{
	uint8 i;
	UART_SendString(dat,len);
	UART_SendString("\r\n",2);
	for(i=0;i<4;i++)
		{
			if(*dat!=UART_Header[i])return ;
			dat++;
		}
	if(*dat=='0')
		{
			LCD1602_AreaClear(0,0,16);
			LCD1602_Show(0,0,dat+2,len-6);
			EEPROM_WriteString(0x40,dat+2,len-6);
			EEPROM_WriteString(0x40+len-6,spaces,22-len);
		}
	else if(*dat=='1')
		{
			LCD1602_AreaClear(0,1,16);
			LCD1602_Show(0,1,dat+2,len-6);
			EEPROM_WriteString(0x80,dat+2,len-6);
			EEPROM_WriteString(0x80+len-6,spaces,22-len);
		}
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
	uint8 temp[20];
	Timer0_Init(1);
	UART_Conf(9600);
	LCD1602_Init();
	EEPROM_ReadString(0x40,16,temp);
	LCD1602_Show(0,0,temp,16);
	EEPROM_ReadString(0x80,16,temp);
	LCD1602_Show(0,1,temp,16);
	while(1)
	{
		UART_Driver();
	}	
}
void it0() interrupt 1
{
	TH0=T0H;
	TL0=T0L;
	UART_RxMonitor(1);
}