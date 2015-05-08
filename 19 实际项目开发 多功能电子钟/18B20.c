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
	2014年12月11日
	功能描述：18B20温度传感器的简单操作
*////////////////////////////////////////////////////////////////////////////////////////
/*//////////////////外部声明//////////////////////////////////
extern int DS18B20_GetTemp(uint8 *addr);    //读取18B20，获得温度。（addr：64字节的器件编号，skip ROM：addr[0]=0x00）返回温度值。
extern void DS18B20_Start(uint8 *addr);     //启动18B20温度转换。（addr：64字节的器件编号，skip ROM：addr[0]=0x00）。每次读出温度后必须重新启动温度转换。
extern bit DS18B20_Init();  //初始化18B20，也可用于检测器件是否存在。返回值1：正常；2：异常。
////////////////////////////////////////////////////////////*/

#include"config.h"

void DelayX10us(unsigned char t)
{
    do {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    } while (--t);
}
bit DS18B20_Init()
{
	bit temp;
	EA=0;
	DS18B20_IO=0;
	DelayX10us(60);
	DS18B20_IO=1;
	DelayX10us(7);
	temp=DS18B20_IO;
	while(!DS18B20_IO);
	EA=1;
	return ~temp;
}
void DS18B20_Write(uint8 dat)
{
	uint8 mask;
	EA=0;
	for(mask=0x01;mask!=0;mask<<=1)
	{
		if(dat&mask)
		{
			DS18B20_IO=0;
			_nop_();
			_nop_();
			DS18B20_IO=1;
			DelayX10us(8);
		}
		else
		{
			DS18B20_IO=0;
			DelayX10us(8);
			DS18B20_IO=1;
		}
		_nop_();
		_nop_();
	}
	EA=1;
}
uint8 DS18B20_Read()
{
	uint8 mask,dat=0;
	EA=0;
	for(mask=0x01;mask!=0;mask<<=1)
	{
		DS18B20_IO=0;
		_nop_();
		_nop_();
		DS18B20_IO=1;
		DelayX10us(1);
		if(DS18B20_IO)dat|=mask;
		DelayX10us(5);
	}
	EA=1;
	return dat;
}
void DS18B20_Start(uint8 *addr)
{
	if(!DS18B20_Init())return ;
	if(addr[0]==0x00)DS18B20_Write(0xCC);
	else
	{
		DS18B20_Write(0x55);
		DS18B20_Write(addr[0]);
		DS18B20_Write(addr[1]);
		DS18B20_Write(addr[2]);
		DS18B20_Write(addr[3]);
		DS18B20_Write(addr[4]);
		DS18B20_Write(addr[5]);
		DS18B20_Write(addr[6]);
		DS18B20_Write(addr[7]);
	}
	DS18B20_Write(0x44);
}
int DS18B20_GetTemp(uint8 *addr)
{
	int temp ;
	uint8 temp1,temp2;
	uint16 temp3;
	if(!DS18B20_Init())return 255;
	if(addr[0]==0x00)DS18B20_Write(0xCC);
	else
	{
		DS18B20_Write(0x55);
		DS18B20_Write(addr[0]);
		DS18B20_Write(addr[1]);
		DS18B20_Write(addr[2]);
		DS18B20_Write(addr[3]);
		DS18B20_Write(addr[4]);
		DS18B20_Write(addr[5]);
		DS18B20_Write(addr[6]);
		DS18B20_Write(addr[7]);
	}
	DS18B20_Write(0xBE);
	temp1=DS18B20_Read();
	temp2=DS18B20_Read();
	temp3=(uint16)temp2;
	temp3<<=8;
	temp3|=(uint16)temp1;
	DelayX10us(15);
	if(temp3&0x8000)
	{
		temp3&=0x07FF;
		temp=-temp3;
	}
	else
	{
		temp3&=0x07FF;
		temp=temp3;
	}
	return temp;
}
