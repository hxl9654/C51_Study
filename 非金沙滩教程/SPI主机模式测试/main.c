#include<reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

extern void SPI_Init();		//SPI初始化（仅主模式使用）
extern void SPI_Write(uint8 dat);	//SPI主模式发送数据（dat：要发送的数据）
extern uint8 SPI_Read();	//SPI主模式读取数据，返回读取到的数据

void main()
{
	uint8 dat;
	SPI_Init();
	SPI_Write(0x96);
	while(1)
	{		
		dat=SPI_Read();
		SPI_Write(dat+1);		
	}		
}