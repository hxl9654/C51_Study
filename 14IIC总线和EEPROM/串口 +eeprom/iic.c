#include<reg51.h>
#include<intrins.h>
#define IICDelay() {nop_();nop_();nop_();nop_();}
sbit IIC_SDA=P3^6;
sbit IIC_SCL=P3^7;
typedef unsigned char uint8;

void IIC_Start()
{
	IIC_SCL=0;
	IICDelay();
	IIC_SDA=1;
	IICDelay();
	IIC_SCL=1;
	IICDelay();
	IIC_SDA=0;
	IICDelay();
	IIC_SCL=0;
}
void IIC_Stop()
{
	IIC_SCL=0;
	IICDelay();
	IIC_SDA=0;
	IICDelay();
	IIC_SCL=1;
	IICDelay();
	IIC_SDA=1;
}
bit IIC_Write(uint8 dat)
{
	uint8 i,mask=0x80;
	bit a;
	for(i=0;i<8;i++)
		{
			if(dat&mask)IIC_SDA=1;
			else IIC_SDA=0;
			IICDelay();
			IIC_SCL=1;
			mask>>=1;
			IIC_SCL=0;
		}
	IICDelay();
	IIC_SDA=1;
	IICDelay();
	IIC_SCL=1;
	IICDelay();
	a=IIC_SDA;
	IIC_SCL=0;
	return (~a); //·µ»Ø1£¬Õý³£
}
uint8 IIC_Read(bit ACK)
{
	uint8 i,mask=0x80,dat=0;
	IIC_SCL=0;
	IIC_SDA=1;
	for(i=0;i<8;i++)
		{
			IICDelay();
			IIC_SCL=1;
			IICDelay();
			if(IIC_SDA)dat|=mask;
			mask>>=1;
			IIC_SCL=0;
		}
	IICDelay();
	IIC_SDA=ACK;
	IICDelay();
	IIC_SCL=1;
	IICDelay();
	IIC_SCL=0;
	return dat;	
}
