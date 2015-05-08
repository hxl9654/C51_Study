/*//////////////////////////GPL开源许可证////////////////////////////////////////////////
    Copyright (C) <2015>  <Xianglong He>
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

	版权所有（C） <2015>   <何相龙>
	这一程序是自由软件，你可以遵照自由软件基金会出版的GNU通用公共许可证
	条款来修改和重新发布这一程序。你可以使用许可证的第三版，或者（根据你
	的选择）用任何更新的版本。

	发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定目的
	的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

	你应该已经和程序一起收到一份GNU通用公共许可证的副本。如果还没有，请访
	问<http://www.gnu.org/licenses/>。

	作者：何相龙
	邮箱：qwgg9654@gmail.com
		  568629794@qq.com
	2015年1月8日
	功能描述：SPI通信的IO口模拟实现
*////////////////////////////////////////////////////////////////////////////////////////
/*//////////////////外部声明//////////////////////////////////
extern void SPI_Init();		//SPI初始化（仅主模式使用）
extern void SPI_Write(uint8 dat);	//SPI主模式发送数据（dat：要发送的数据）
extern uint8 SPI_Read();	//SPI主模式读取数据，返回读取到的数据
extern void SPI_Slave_Send(uint8 dat);	//SPI从模式回应数据（dat：要回应的数据）
extern uint8 SPI_Slave_Resive();	//SPI从模式接收数据，返回接收到的数据
////////////////////////////////////////////////////////////*/

#include<reg52.h>
#include<intrins.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

#define CPOL 1	  //设置SPI通信模式（建议：如果要用SPI做多机通信，不想作死的话，建议CPHA=1）
#define CPHA 1
#define Dealy(); {_nop_();_nop_();_nop_();_nop_();_nop_();}	//根据实际要求修改
sbit CS=P1^4;	//SPI通信的四个IO口
sbit SCLK=P1^1;
sbit MOSI=P1^0;
sbit MISO=P1^2;

void SPI_Init()
{
	SCLK=CPOL;
	CS=1;
}
void SPI_Write(uint8 dat)
{
	uint8 mask;
	CS=0;
	if(CPHA)
		{						
			for(mask=0x80;mask!=0;mask>>=1)
				{
					SCLK=~SCLK;
					MOSI=dat&mask;
					Dealy();				
					SCLK=~SCLK;
					Dealy();
				}
		}
	else 
		{
			for(mask=0x80;mask!=0;)
				{				
					MOSI=dat&mask;
					SCLK=~SCLK;
					Dealy();
					mask>>=1;				
					SCLK=~SCLK;
					Dealy();
				}
		}
	CS=1;	
}
uint8 SPI_Read()
{
	uint8 dat,mask=0x80;
	CS=0;
		if(CPHA)
		{
			for(;mask!=0;mask>>=1)
				{
					SCLK=~SCLK;
					Dealy();
					SCLK=~SCLK;
					if(MISO)dat|=mask;
					Dealy();
				}
		}
	else 
		{
	        for(;mask!=0;mask>>=1)
		        {            
					if(MISO)dat|=mask;
					SCLK=~SCLK;
					Dealy();
		            SCLK=~SCLK;
					Dealy();	            
		        }
	    }
	CS=1;
	return dat;
}
void SPI_Slave_Send(uint8 dat)
{
	uint8 mask=0x80;	
	if(CPHA)
		{
			while(CS);
			for(;mask!=0;mask>>=1)
				{
					while(SCLK==CPOL);
					MISO=dat&mask;
					while(SCLK!=CPOL);	
				}
		}
	else
		{
			for(;mask!=0;mask>>=1)
				{				
					MISO=dat&mask;
					while(CS);
					while(SCLK==CPOL);
					while(SCLK!=CPOL);	
				}
		} 
		
}
uint8 SPI_Slave_Resive()
{
	uint8 mask=0x80,dat;
	while(CS);
	if(CPHA)
		{
			for(;mask!=0;mask>>=1)
				{
					while(SCLK==CPOL);
					while(SCLK!=CPOL);
					if(MOSI)dat|=mask;
				}			
		}
	else
		{
			for(;mask!=0;mask>>=1)
				{
					while(SCLK==CPOL);
					if(MOSI)dat|=mask;					
					while(SCLK!=CPOL);					
				}	
		}
   return dat;
}