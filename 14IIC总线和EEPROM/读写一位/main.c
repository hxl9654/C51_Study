#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
extern uint8 IIC_Read(bit ACK);
extern bit IIC_Write(uint8 dat);
extern void IIC_Start();
extern void IIC_Stop();
extern void lcdinit();
extern void lcdshow(uint8 x,uint8 y,uint8 *str,uint8 n);
uint8 e2read(uint8 addr)
{
	uint8 dat;
	IIC_Start();
	IIC_Write(0x50<<1);
	IIC_Write(addr);
	IIC_Start();
	IIC_Write((0x50<<1)|0x01);
	dat=IIC_Read(1);
	IIC_Stop();
	return dat;
}
void e2write(uint8 addr,uint8 dat)
{
	IIC_Start();
	IIC_Write(0x50<<1);
	IIC_Write(addr);
	IIC_Write(dat);
	IIC_Stop();
}
void main()
{
	uint8 dat;
	uint8 d[3];
	lcdinit();
	dat=e2read(0x02);
	d[2]=dat%10+'0';
	d[1]=dat/10%10+'0';
	d[0]=dat/100+'0';
	lcdinit();
	lcdshow(0,0,d,3);

	e2write(0x02,dat+1);
	while(1);
}