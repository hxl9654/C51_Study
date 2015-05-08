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
	2014年11月11日
	功能描述：51单片机模拟串口
*////////////////////////////////////////////////////////////////////////////////////////
#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
bit rend=0,send=0;
uint8 a=0,d=0,rs=0;
void setbaud(baud)
{
	TH0=256-(11059200/12)/baud;
	TL0=TH0;
}
void uartresive()
{
	setbaud(9600);
	rs=0;
	TL0=256-(256-TH0)/2+24;
	while(RXD);
	TR0=1;
}

void uartsend(uint8 da)
{
	setbaud(9600);
	d=da;
	rs=1;
	TXD=1;
	TXD=0;
	TR0=1;
}
void main()
{
	EA=1;
	ET0=1;
	TMOD=0X02;
	while(1)
	{
		uartresive();
		while(!rend);
		rend=0;
		uartsend(a+1);
		while(!send);
		send=0;
	}
}
void t0i() interrupt 1
{
	static uint8 i=0;
	if(rs)
		{
			if(i<=8)
				{
					TXD=d&0x01;
					d>>=1;
				}
			else if(i==9)
				{
					TXD=1;
				}
			else
				{
					TR0=0;
					i=0;
					d=0;
					send=1;
				}
		}
	else
		{
			if(i==0||i==1);
			else if(i<=9)
				{
					a>>=1;
					if(RXD)a|=0x80;
				}
			else if(RXD==1)
				{
					rend=1;
					i=0;
					TR0=0;
				}
		}
	i++;
}
