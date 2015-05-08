#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

extern void LCD1602_Show(uint8 x,uint8 y,uint8 *dat,uint8 len); //LCD1602显示字符串（x：横坐标，y：纵坐标，dat：要显示的字符数组，len：要显示的长度）
extern void LCD1602_Init(); //LCD1602初始化函数，在主函数开头调用。

extern void IIC_Start();    //发出IIC通信起始信号
extern void IIC_Stop();     //发出IIC通信终止信号
extern bit IIC_Write(uint8 dat);    //IIC通信写入一个uint8型的数据（dat：要写入的数据）返回：1，成功；0，失败
extern uint8 IIC_Read(bit ACK);     //IIC通信读出一个uint8型的数据（ACK：0，非末位数据；1：末位数据），返回读出的数据
extern uint8 PCF8591_Read(uint8 chanl);	//读取AD转换得到的值（普通转换，非差分）（chanl：通道号）返回，AD值

void main()
{
	uint8 dat;
	uint16 temp;
	uint8 LcdBuff[6];
	LCD1602_Init();
	while(1)
	{
		dat=PCF8591_Read(0);
		temp=dat*125/64;
		LcdBuff[0]=temp/100+'0';
	    LcdBuff[1]='.';
		LcdBuff[2]=temp/10%10+'0';
		LcdBuff[3]=temp%10+'0';
		LcdBuff[4]='V';
		LcdBuff[5]='\0';
		LCD1602_Show(0,0,LcdBuff,5);		
	}
}