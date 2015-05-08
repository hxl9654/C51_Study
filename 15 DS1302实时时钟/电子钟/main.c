#include<reg51.h>
#include"main.h"

struct StructTime{
	uint8 sec;
	uint8 min;
	uint8 hour;
	uint8 day;
	uint8 month;
	uint8 week;
	uint8 year;
}; 
struct StructTime time;
bit Flag_200ms=0;
uint8 TimeSetIndex=0;
uint8 T0H,T0L;
 
void main()
{
	
	LCD1602_Init();	
	Timer0_Init(1);
	time.day=1;
	time.month=1;
	time.week=1;
	DS1302_Set();
	while(1)
	{
		KeyDriver();
		if(Flag_200ms)
			{
				Flag_200ms=0;
				if(TimeSetIndex==0)
					TimeRefresh();
			}
	}
}
void RefreshCursor()
{
	uint8 x,y;
	switch(TimeSetIndex)
	{
		case 0:x=0;y=0;break;
		case 1:x=2;y=0;break;
		case 2:x=3;y=0;break;
		case 3:x=5;y=0;break;
		case 4:x=6;y=0;break;
		case 5:x=8;y=0;break;
		case 6:x=9;y=0;break;
		case 7:x=15;y=0;break;
		case 8:x=4;y=1;break;
		case 9:x=5;y=1;break;
		case 10:x=7;y=1;break;
		case 11:x=8;y=1;break;
		case 12:x=10;y=1;break;
		case 13:x=11;y=1;break;
	}
	LCD1602_SetLocation(x,y);
}
void ExitTimeSet(bit flag)
{
	TimeSetIndex=0;
	LCD1602_CloseCursor();
	RefreshCursor();
	if(flag)DS1302_Set();
}
void KeyAction(uint8 keyc,uint8 statu)	//当按键被触发，自动调用（keyc：按键码，statu：按键状态，0-按键按下，1-按键弹起，2-按键长按）
{
	if(statu==1)return ;
	if(keyc==0x26)	//up
	{
		switch(TimeSetIndex)
		{
			case 1:if(time.year<90)time.year+=10;else time.year-=90;break;
			case 2:if(time.year<99)time.year+=1;else time.year=0;break;
			case 3:if(time.month<=2)time.month+=10;break;
			case 4:if(time.month<=11)time.month+=1;else time.month=1;break;
			case 5:if(time.day<=21)time.day+=10;else time.day-=20;break;
			case 6:if(time.day<=30)time.day+=1;else time.day=1;break;
			case 7:if(time.week<=6)time.week+=1;else time.week=1;break;
			case 8:if(time.hour<=13)time.hour+=10;break;
			case 9:if(time.hour<=22)time.hour+=1;else time.hour=0;break;
			case 10:if(time.min<50)time.min+=10;else time.min-=50;break;
			case 11:if(time.min<=58)time.min+=1;else time.min=0;break;
			case 12:if(time.sec<50)time.sec+=10;else time.sec-=50;break;
			case 13:if(time.sec<=58)time.sec+=1;else time.sec=0;break;
		}
		DS1302_Set();
		TimeRefresh();	
	}
	else if(keyc==0x28)	//down
	{
		switch(TimeSetIndex)
		{
			case 1:if(time.year>=10)time.year-=10;else time.year+=90;break;
			case 2:if(time.year>0)time.year-=1;else time.year=99;break;
			case 3:if(time.month>10)time.month-=10;break;
			case 4:if(time.month>1)time.month-=1;else time.month=12;break;
			case 5:if(time.day>10)time.day-=10;else time.day+=20;break;
			case 6:if(time.day>1)time.day-=1;else time.day=31;break;
			case 7:if(time.week>1)time.week-=1;else time.week=7;break;
			case 8:if(time.hour>=10)time.hour-=10;else time.hour+=10;break;
			case 9:if(time.hour>=1)time.hour-=1;else time.hour=23;break;
			case 10:if(time.min>=10)time.min-=10;else time.min+=50;break;
			case 11:if(time.min>=1)time.min-=1;else time.min=59;break;
			case 12:if(time.sec>=10)time.sec-=10;else time.sec+=50;break;
			case 13:if(time.sec>=1)time.sec-=1;else time.sec=59;break;
		}
		DS1302_Set();
		TimeRefresh();	
	}
	else if(keyc==0x25)	//left
		{
			if(TimeSetIndex>1)TimeSetIndex--;
			else TimeSetIndex=13;
			RefreshCursor();
		}
	else if(keyc==0x27)	//right
		{
			if(TimeSetIndex<13)TimeSetIndex++;
			else TimeSetIndex=1;
			RefreshCursor();
		}
	else if(keyc==0x0d)	//enter
		{
			if(TimeSetIndex==0)
				{
					TimeSetIndex=1;
					LCD1602_ShowCursor();
					RefreshCursor();
				}
			else 
				{
					ExitTimeSet(1);
				}	
		}
	else if(keyc==0x1b)	//esc
		{
			ExitTimeSet(0);
		}
}
void LCD1602_Refresh()
{
	uint8 temp[16];
	temp[0]='2';
	temp[1]='0';
	temp[2]=time.year/10+'0';
	temp[3]=time.year%10+'0';
	temp[4]='-';
	temp[5]=time.month/10+'0';
	temp[6]=time.month%10+'0';
	temp[7]='-';
	temp[8]=time.day/10+'0';
	temp[9]=time.day%10+'0';
	temp[10]=' ';
	temp[11]='w';
	temp[12]='e';
	temp[13]='e';
	temp[14]='k';
	temp[15]=time.week+'0';
	LCD1602_Show(0,0,temp,16);
	temp[0]=' ';
	temp[1]=' ';
	temp[2]=' ';
	temp[3]=' ';
	temp[4]=time.hour/10+'0';
	temp[5]=time.hour%10+'0';
	temp[6]=':';
	temp[7]=time.min/10+'0';
	temp[8]=time.min%10+'0';
	temp[9]=':';
	temp[10]=time.sec/10+'0';
	temp[11]=time.sec%10+'0';
	temp[12]=' ';
	temp[13]=' ';
	temp[14]=' ';
	temp[15]=' ';
	LCD1602_Show(0,1,temp,16);
}
void TimeRefresh()
{
	uint8 dat[8];
	DS1302_BrustRead(dat);
	ToUint8(dat);
	LCD1602_Refresh();
}
void ToUint8(uint8 *dat)
{
	time.sec=((dat[0]>>4)&0x07)*10;
	time.sec+=(dat[0]&0x7F)%16;
	time.min=dat[1]/16*10+dat[1]%16;
	time.hour=((dat[2]>>4)&0x07)*10;
	time.hour+=(dat[2]&0x7F)%16;
	time.day=dat[3]/16*10+dat[3]%16;	
	time.month=dat[4]/16*10+dat[4]%16;	
	time.year=dat[6]/16*10+dat[6]%16;
	time.week=dat[5];
}
void ToBCD(uint8 *dat)
{
	dat[7]=0x00;
	dat[0]=(time.sec%10)+(time.sec/10*0x10);
	dat[1]=(time.min%10)+(time.min/10*0x10);
	dat[2]=(time.hour%10)+(time.hour/10*0x10);
	dat[3]=(time.day%10)+(time.day/10*0x10);
	dat[4]=(time.month%10)+(time.month/10*0x10);
	dat[6]=(time.year%10)+(time.year/10*0x10);
	dat[5]=time.week;
}
void DS1302_Set()
{
	uint8 dat[8];
	ToBCD(dat);
	DS1302_Init(dat);
}
void Timer0_Init(uint8 ms)
{
	uint16 temp;
	temp=65536-ms*110592/120;
	temp+=12;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)(temp&0x0F);
	TH0=T0H;
	TL0=T0L;
	ET0=1;
	EA=1;
	TMOD&=0XF0;
	TMOD|=0X01;
	TR0=1;
}
void Interrupt_Timer0() interrupt 1
{
	static uint8 ms200=0;
	TH0=T0H;
	TL0=T0L;
	ms200++;
	if(ms200>=200)
	{
		ms200-=200;
		Flag_200ms=1;
	}
	KeyScan();	
}