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

	版权所有（C） <2014>   <何相龙>
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
	2014年12月9日
	功能描述：串口字符串通信
*////////////////////////////////////////////////////////////////////////////////////////
/*//////////////////外部声明//////////////////////////////////
extern void UART_RxMonitor(uint8 ms);//UART字符串接收结束判断。在定时器中调用（ms：定时器时钟周期）
extern void UART_Driver();//UART通信监控函数，在主循环中调用。如果接收到字符串，会自动调用另行编写的UART_Action(uint8 *dat,uint8 len)
extern void UART_SendString(uint8 *dat,uint8 len);  //串口通信：发送一个字符串（dat：要发送的字符串，len：字符串长度）
extern void UART_Conf(uint16 baud); //UART设置函数（buad：欲设置的波特率）
////////////////////////////////////////////////////////////*/
#include<reg51.h>
#define BUFFMAX 64
typedef unsigned char uint8;
typedef unsigned int uint16;
extern void UART_Action(uint8 *dat,uint8 len);  //此函数须另行编写：当串口
uint8 pdata UART_Buff[BUFFMAX];
uint8 UART_BuffIndex=0;
bit UART_SendFlag,UART_ResiveFlag,UART_ResiveStringEndFlag,UART_ResiveStringFlag;
void UART_Conf(uint16 baud) //UART设置函数（buad：欲设置的波特率）
{
	TL1=TH1=256-11059200/12/32/baud;
	EA=1;
	ES=1;
	TMOD&=0X0F;
	TMOD|=0X20;
	SCON=0X50;
	TR1=1;
}
void UART_SendString(uint8 *dat,uint8 len)  //串口通信：发送一个字符串（dat：要发送的字符串，len：字符串长度）
{
	while(len)
	{
		len--;
		SBUF=*dat;
		dat++;
		while(!UART_SendFlag);
		UART_SendFlag=0;
	}
}
uint8 UART_Read(uint8 *to,uint8 len)    //UART读取函数，从SBUF读取到数据暂存数组
{
	uint8 i;
	if(UART_BuffIndex<len)len=UART_BuffIndex;
	for(i=0;i<len;i++)
		{
			*to=UART_Buff[i];
			to++;
		}
	UART_BuffIndex=0;
	return len;
}
void UART_Driver()//UART通信监控函数，在主循环中调用。如果接收到字符串，会自动调用另行编写的UART_Action(uint8 *dat,uint8 len)
{
	uint8 pdata dat[64];
	uint8 len;
	if(UART_ResiveStringEndFlag)
		{
			UART_ResiveStringEndFlag=0;
			len=UART_Read(dat,64);
			UART_Action(dat,len);
		}
}
void UART_RxMonitor(uint8 ms)//UART字符串接收结束判断。在定时器中调用（ms：定时器时钟周期）
{
	static uint8 ms30=0,UART_BuffIndex_Backup;
	ms30+=ms;
	if(!UART_ResiveStringFlag)return ;
	if(UART_BuffIndex_Backup!=UART_BuffIndex)
	{
		UART_BuffIndex_Backup=UART_BuffIndex;
		ms30=0;
	}
	if(ms30>30)
		{
			ms30=0;
			UART_ResiveStringEndFlag=1;
			UART_ResiveStringFlag=0;
		}

}
void interrupt_UART() interrupt 4   //串口中断函数
{
	if(TI)
	{
		TI=0;
		UART_SendFlag=1;
	}
	if(RI)
	{
		RI=0;
		UART_ResiveFlag=1;
		UART_Buff[UART_BuffIndex]=SBUF;
		UART_ResiveStringFlag=1;
		UART_BuffIndex++;
	}
}
