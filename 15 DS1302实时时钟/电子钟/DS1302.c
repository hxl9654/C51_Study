/*//////////////////////////GPL开源许可证////////////////////////////////////////////////
    Copyright (C) <2014>  <Xianglong He>
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	作者：何相龙
	邮箱：qwgg9654@gmail.com
		  568629794@qq.com
	2014年12月9日
	功能描述：DS1302时钟芯片的时间设置及读取
*////////////////////////////////////////////////////////////////////////////////////////
/*//////////////////外部声明//////////////////////////////////
extern void DS1302_Init(uint8 *dat);  //DS1302初始化函数，在主函数开头调用，dat是设置的时间
extern uint8 DS1302_SingleRead(uint8 addr); //DS1302单字节读取，dat为读取数据存储位置，addr为寄存器地址，详见数据手册
extern void DS1302_SingleWrite(uint8 addr,uint8 dat);   //DS1302单字节写入，dat为写入数据，addr为寄存器地址，详见数据手册
extern void DS1302_BrustWrite(uint8 *dat);  //DS1302Brust模式写入，dat为uint8型的拥有8个元素的数组
extern void DS1302_BrustRead(uint8 *dat);   //DS1302Brust模式读取，dat为uint8型的拥有8个元素的数组
////////////////////////////////////////////////////////////*/
#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

struct StructTime{
	uint8 sec;
	uint8 min;
	uint8 hour;
	uint8 day;
	uint8 month;
	uint8 week;
	uint8 year;
};

sbit DS1302_CE=P1^7;//请根据实际情况修改
sbit DS1302_IO=P3^4;
sbit DS1302_CK=P3^5;

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

void DS1302_ByteWrite(uint8 dat)
{
	uint8 mask;
	for(mask=0x01;mask!=0;mask<<=1)
		{
			if(mask&dat)DS1302_IO=1;
			else DS1302_IO=0;
			DS1302_CK=1;
			DS1302_CK=0;
		}
	DS1302_IO=1;
}

uint8 DS1302_ByteRead()
{
	uint8 mask,dat=0;
	DS1302_IO=1;
	for(mask=0x01;mask!=0;mask<<=1)
		{
			if(DS1302_IO)dat|=mask;
			DS1302_CK=1;
			DS1302_CK=0;
		}
	return dat;
}
uint8 DS1302_SingleRead(uint8 addr)		//DS1302单字节读取，dat为读取数据存储位置，addr为寄存器地址，详见数据手册
{
	uint8 dat;
	EA=0;
	DS1302_CE=1;
	DS1302_ByteWrite(0x81|(addr<<1));
	dat=DS1302_ByteRead();
	DS1302_CE=0;
	EA=1;
	return dat;
}
void DS1302_SingleWrite(uint8 addr,uint8 dat)	//DS1302单字节写入，dat为写入数据，addr为寄存器地址，详见数据手册
{
	EA=0;
	DS1302_CE=1;
	DS1302_ByteWrite(0x80|(addr<<1));
	DS1302_ByteWrite(dat);
	DS1302_CE=0;
	EA=1;
}
void DS1302_BrustWrite(uint8 *dat)	//DS1302Brust模式写入，dat为uint8型的拥有8个元素的数组
{
	uint8 i;
	EA=0;
	DS1302_CE=1;
	DS1302_ByteWrite(0xBE);
	for(i=0;i<8;i++)
		{
			DS1302_ByteWrite(*dat);
			dat++;
		}
	DS1302_CE=0;
	EA=1;
}
void DS1302_BrustRead(uint8 *dat)	//DS1302Brust模式读取，dat为uint8型的拥有8个元素的数组
{
	uint8 i;
	EA=0;
	DS1302_CE=1;
	DS1302_ByteWrite(0xBF);
	for(i=0;i<8;i++)
		{
			*dat=DS1302_ByteRead();
			dat++;
			Delay1ms();
		}
	DS1302_IO=0;
	DS1302_CE=0;
	EA=1;
}
void DS1302_Init(uint8 *dat)	//DS1302初始化函数，在主函数开头调用，dat是设置的时间
{
	DS1302_CE=0;
	DS1302_CK=0;
	DS1302_IO=1;
	DS1302_SingleWrite(7, 0x00);
    DS1302_BrustWrite(dat);
}
/*//////////////////示例程序///////////////////////////////

void main()
{
	uint8 dat[8];
	uint8 dat[8]={0x00,0x15,0x91,0x09,0x12,0x02,0x14,0x00};	  //初始时间
	uint8 temp[16];
	uint8 backup=0xAA;
	LCD1602_Init();
	Timer0_Conf(1);
	DS1302_Init(dat);
	while(1)
	{
		if(Flag_200ms)
		{
			Flag_200ms=0;
			DS1302_BrustRead(dat);
			if(dat[0]==backup)continue;
			backup=dat[0];
			temp[0]='2';
			temp[1]='0';
			temp[2]=(dat[6]>>4)+'0';
			temp[3]=(dat[6]&0x0F)+'0';
			temp[4]='-';
			temp[5]=(dat[4]>>4)+'0';
			temp[6]=(dat[4]&0x0F)+'0';
			temp[7]='-';
			temp[8]=(dat[3]>>4)+'0';
			temp[9]=(dat[3]&0x0F)+'0';
			temp[10]=' ';
			temp[11]='w';
			temp[12]='e';
			temp[13]='e';
			temp[14]='k';
			temp[15]=dat[5]+'0';
			LCD1602_Show(0,0,temp,16);
			temp[0]=' ';
			temp[1]=' ';
			temp[2]=' ';
			temp[3]=' ';
			temp[4]=((dat[2]>>4)&0x07)+'0';
			temp[5]=(dat[2]&0x0F)+'0';
			temp[6]=':';
			temp[7]=(dat[1]>>4)+'0';
			temp[8]=(dat[1]&0x0F)+'0';
			temp[9]=':';
			temp[10]=((dat[0]>>4)&0x07)+'0';
			temp[11]=(dat[0]&0x0F)+'0';
			temp[12]=' ';
			temp[13]=' ';
			temp[14]=' ';
			temp[15]=' ';
			LCD1602_Show(0,1,temp,16);
		}
	}
}
void it0() interrupt 1
{
	static uint8 ms200=0;
	TH0=T0H;
	TL0=T0L;
	ms200++;
	if(ms200>=200)
		{
			ms200=0;
			Flag_200ms=1;
		}
}
/////////////////////////////////////////////////////////*/
