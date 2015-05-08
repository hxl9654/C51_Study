#include<reg51.h>
#include<intrins.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
extern void IIC_Start();
extern void IIC_Stop();
extern bit IIC_Write(uint8 dat);
extern uint8 IIC_Read(bit ACK);
uint8 EEPROM_Read(uint8 addr)
{
	uint8 temp;
	IIC_Start();
	IIC_Write(0x50<<1);
	IIC_Write(addr);
	IIC_Start();
	IIC_Write((0x50<<1)|0x01);
	temp=IIC_Read(1);
	IIC_Stop();
	return temp;	
}
void EEPROM_ReadString(uint8 addr,uint8 len,uint8 *dat)
{
	IIC_Start();
	IIC_Write(0x50<<1);
	IIC_Write(addr);
	IIC_Start();
	IIC_Write((0x50<<1)|0x01);	
	while(len>0)
	{
		*dat=IIC_Read(0);
		dat++;
		len--;
	}
	*dat=IIC_Read(1);
	IIC_Stop();	
}
void EEPROM_Write(uint8 addr,uint8 dat)
{
	bit temp;
	while(1)		   
	{
		IIC_Start();
		temp=IIC_Write(0x50<<1);
		if(temp)break;
		else IIC_Stop();
	}
	IIC_Write(addr);
	IIC_Write(dat);
	IIC_Stop();
}
void EEPROM_WriteString(uint8 addr,uint8 *dat,uint8 len)
{
	bit temp;
	while(len>0)
	{
		while(1)		   
		{
			IIC_Start();
			temp=IIC_Write(0x50<<1);
			if(temp)break;
			else IIC_Stop();
		}
		IIC_Write(addr);
		while(len>0)
		{
			IIC_Write(*dat);
			len--;
			addr++;
			dat++;
			if((addr&0x07)==0)break;	
		}		
		IIC_Stop();
	}
}