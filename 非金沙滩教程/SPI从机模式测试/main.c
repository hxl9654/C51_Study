#include<reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

sbit CS=P1^4;
extern void SPI_Slave_Send(uint8 dat);	//SPI从模式回应数据（dat：要回应的数据）
extern uint8 SPI_Slave_Resive();	//SPI从模式接收数据，返回接收到的数据

void main()
{
	uint8 dat;
	while(1)
	{
		//for(dat=0;;dat++)
		//	SPI_Slave_Send(dat);
		dat=SPI_Slave_Resive();
		while(!CS);
		SPI_Slave_Send(dat+1);

		/*SPI_Slave_Send(0x96);
		SPI_Slave_Send(0x54);
		SPI_Slave_Send(0x07);
		SPI_Slave_Send(0x31);	 */
	}		
}