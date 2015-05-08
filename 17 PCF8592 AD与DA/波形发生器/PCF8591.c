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
	功能描述：PCF8591 AD/DA芯片简单控制
*////////////////////////////////////////////////////////////////////////////////////////
/*//////////////////外部声明//////////////////////////////////
extern uint8 PCF8591_Read(uint8 chanl);	//读取AD转换得到的值（普通转换，非差分）（chanl：通道号）返回，AD值
extern void PCF8591_DA(uint8 dat);	//启动DA输出（dat：要输出的AD值）。
////////////////////////////////////////////////////////////*/
#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

extern void IIC_Start();    //发出IIC通信起始信号
extern void IIC_Stop();     //发出IIC通信终止信号
extern bit IIC_Write(uint8 dat);    //IIC通信写入一个uint8型的数据（dat：要写入的数据）返回：1，成功；0，失败
extern uint8 IIC_Read(bit ACK);     //IIC通信读出一个uint8型的数据（ACK：0，非末位数据；1：末位数据），返回读出的数据

uint8 PCF8591_Read(uint8 chanl)
{
	uint8 dat;
	IIC_Start();
	IIC_Write(0x90);
	IIC_Write(chanl<<4);
	IIC_Start();
	IIC_Write(0x91);
	IIC_Read(0);
	dat=IIC_Read(1);
	IIC_Stop();
	return dat;
}
void PCF8591_DA(uint8 dat)
{
	IIC_Start();
	IIC_Write(0x90);
	IIC_Write(0x40);
	IIC_Write(dat);
	IIC_Stop();
}
