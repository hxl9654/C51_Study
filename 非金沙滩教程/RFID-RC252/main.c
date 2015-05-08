#include<reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
extern void IIC_Start();    //发出IIC通信起始信号
extern void IIC_Stop();     //发出IIC通信终止信号
extern bit IIC_Write(uint8 dat);    //IIC通信写入一个uint8型的数据（dat：要写入的数据）返回：1，成功；0，失败
extern uint8 IIC_Read(bit ACK);     //IIC通信读出一个uint8型的数据（ACK：0，非末位数据；1：末位数据），返回读出的数据
void UART_Init(uint16 baud)
{
	uint16 temp;
	temp=256-11059200/32/12/baud;
	TH1=TL1=baud;
	TMOD=0X20;
	EA=1;
	ES=1;
	TR1=1;
}
void main()
{
	UART_Init(9600);
	while(1);
}
void IIC_Send(uint8 dat)
{
	IIC_Start();
	IIC_Write(0x50);
	IIC_Write(dat);
	IIC_Stop();
}
uint8 IIC_Resive()
{
	
}
void Interrupt_UART() interrupt 4
{
	if(RI)
	{
		RI=0;
		IIC_Sned(SBUF);
		SBUF=IIC_Resive();
	}
	else TI=0;
}
