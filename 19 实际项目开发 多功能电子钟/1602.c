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
	功能描述：1602液晶的显示及设置
*////////////////////////////////////////////////////////////////////////////////////////
/*//////////////////外部声明//////////////////////////////////
extern void LCD1602_Clear();    //LCD1602清屏
extern void LCD1602_AreaClear(uint8 x,uint8 y,uint8 len);   //LCD1602范围清屏（x：横坐标，y：纵坐标，len：清屏的字符数）
extern void LCD1602_Show(uint8 x,uint8 y,uint8 *dat,uint8 len); //LCD1602显示字符串（x：横坐标，y：纵坐标，dat：要显示的字符数组，len：要显示的长度）
extern void LCD1602_Init(); //LCD1602初始化函数，在主函数开头调用。
extern void LCD1602_CloseCursor();	//关闭LCD1602光标
extern void LCD1602_ShowCursor();	//打开LCD1602光标
extern void LCD1602_SetLocation(uint8 x,uint8 y);	//设置LCD1602光标位置
////////////////////////////////////////////////////////////*/
#include"config.h"
extern bit LED_Refresh;
void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}
void LCD1602_Wait()
{
	uint8 a;
	LCD1602_RS=0;
	LCD1602_RW=1;
	LCD1602_DB=0xFF;
	do
	{
		LCD1602_E=1;
		a=LCD1602_DB;
		LCD1602_E=0;
	}while(a&0x80);
}
void LCD1602_WriteData(uint8 dat)
{	
	LED_Refresh=0;
	ENLED=1;
	LCD1602_Wait();
	LCD1602_RW=0;
	LCD1602_RS=1;
	LCD1602_DB=dat;
	LCD1602_E=1;
	LCD1602_E=0;
	LED_Refresh=1;
}
void LCD1602_WriteCMD(uint8 cmd)
{
	LED_Refresh=0;
	ENLED=1;
	LCD1602_Wait();
	LCD1602_RW=0;
	LCD1602_RS=0;
	LCD1602_DB=cmd;
	LCD1602_E=1;
	LCD1602_E=0;
	LED_Refresh=1;
}
void LCD1602_Clear()
{
	LCD1602_WriteCMD(0x01);
}
void LCD1602_SetLocation(uint8 x,uint8 y)
{
	uint8 temp;
	if(y==1)temp=0x40;
	else temp=0x00;
	temp+=x;
	LCD1602_WriteCMD(temp|0x80);
}
void LCD1602_AreaClear(uint8 x,uint8 y,uint8 len)
{
	LCD1602_SetLocation(x,y);
	while(len--)
	{
		LCD1602_WriteData(' ');
	}
}
void LCD1602_Show(uint8 x,uint8 y,uint8 *dat,uint8 len)
{
	LCD1602_SetLocation(x,y);
	while(len--)
	{
		LCD1602_WriteData(*dat++);
	}
}
void LCD1602_ShowCursor()
{
    LCD1602_WriteCMD(0x0F);
}
void LCD1602_CloseCursor()
{
    LCD1602_WriteCMD(0x0C);
}
void LCD1602_Init()
{
	LCD1602_WriteCMD(0x38);
	Delay5ms();
	LCD1602_WriteCMD(0x38);
	Delay5ms();
	LCD1602_WriteCMD(0x38);
	Delay5ms();
	LCD1602_WriteCMD(0x38);
	LCD1602_WriteCMD(0x0C);
	LCD1602_WriteCMD(0x06);
	LCD1602_WriteCMD(0x01);
}
