#include"main.h"
uint8 TimeSetIndex=0;
uint8 T0H,T0L;
int wendu=0;		//温度 
uint8 LedBuff[2][8];
bit BUZZ_Run=0;
uint8 TimeSetMainIndex=0;
void main()
{
	uint8 a[1]=0x00;
	DS18B20_Start(a);
	LCD1602_Init();
	INT0_Init();	
	Timer0_Init(1);
	Timer1_Init();
	Timer2_Init(1);
	time.day=1;
	time.month=1;
	time.week=1;
	DS1302_Set();
	TimeRefresh(1);
	EA=1;
	while(1)
	{
		KeyDriver();
		if(Flag_200ms)
			{
				Flag_200ms=0;
				if(TimeSetIndex==0)
					TimeRefresh(0);
				if(Flag_2s)
				{
					Flag_2s=0;
					wendu=DS18B20_GetTemp(a);
					DS18B20_Start(a);
					if(TimeSetIndex==0)LCD1602_Refresh();
				}
				if(Flag_IR)
				{
					Flag_IR=0;
					if(IRBuff[0]==0x00)IR_Action(IRBuff[2]);
				}
			}
	}
}
void IR_Action(uint8 IRCode)
{
	if(IRCode==0x47)KeyAction(0x1B,0);
	else if(IRCode==0x40)KeyAction(0x25,0);
	else if(IRCode==0x43)KeyAction(0x27,0);	
	else if(IRCode==0x15)KeyAction(0x28,0);	
	else if(IRCode==0x09)KeyAction(0x26,0);	
	else if(IRCode==0x16)KeyAction(0x30,0);
	else if(IRCode==0x19)KeyAction(0x1B,0);
	else if(IRCode==0x0D)KeyAction(0x0D,0);	
	else if(IRCode==0x0C)KeyAction(0x31,0);	
	else if(IRCode==0x18)KeyAction(0x32,0);	
	else if(IRCode==0x5E)KeyAction(0x33,0);
	else if(IRCode==0x08)KeyAction(0x34,0);	
	else if(IRCode==0x1C)KeyAction(0x35,0);	
	else if(IRCode==0x5A)KeyAction(0x36,0);	
	else if(IRCode==0x42)KeyAction(0x37,0);	
	else if(IRCode==0x52)KeyAction(0x38,0);	
	else if(IRCode==0x4A)KeyAction(0x39,0);				
}
void RefreshCursor()
{
	uint8 x,y=1;
	switch(TimeSetIndex)
	{
		case 1:x=2;break;
		case 2:x=3;break;
		case 3:x=5;break;
		case 4:x=6;break;
		case 5:x=8;break;
		case 6:x=9;break;
		case 7:x=15;break;
		case 8:x=0;break;
		case 9:x=1;break;
		case 10:x=3;break;
		case 11:x=4;break;
		case 12:x=6;break;
		case 13:x=7;break;
		case 14:x=0;break;
		case 15:x=1;break;
		case 16:x=3;break;
		case 17:x=4;break;
		case 18:x=6;break;
		case 19:x=7;break;
		case 20:x=15;break;
	}
	LCD1602_SetLocation(x,y);
}
void ExitTimeSet(bit flag)
{
	TimeSetIndex=0;
	TimeSetMainIndex=0;
	LCD1602_CloseCursor();
	RefreshCursor();
	if(flag)DS1302_Set();
	TimeRefresh(1);
}
void SetTimeMainIndex(uint8 index)
{
	uint8 temp[16];
	if(index==1)
	{
		TimeSetMainIndex=1;		
		LCD1602_Clear();
		LCD1602_Show(0,0,"Set Date:",9);
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
		LCD1602_Show(0,1,temp,16);
	}
	else if(index==2)
	{
		TimeSetMainIndex=2;
		LCD1602_Clear();
		LCD1602_Show(0,0,"Set Time:",9);					
		temp[0]=time.hour/10+'0';
		temp[1]=time.hour%10+'0';
		temp[2]=':';
		temp[3]=time.min/10+'0';
		temp[4]=time.min%10+'0';
		temp[5]=':';
		temp[6]=time.sec/10+'0';
		temp[7]=time.sec%10+'0';
		LCD1602_Show(0,1,temp,8);
	}
	else if(index==3)
	{
		TimeSetMainIndex=3;
		LCD1602_Clear();
		LCD1602_Show(0,0,"Set Alarm Clock:",16);					
		temp[0]=alarm.hour/10+'0';
		temp[1]=alarm.hour%10+'0';
		temp[2]=':';
		temp[3]=alarm.min/10+'0';
		temp[4]=alarm.min%10+'0';
		temp[5]=':';
		temp[6]=alarm.sec/10+'0';
		temp[7]=alarm.sec%10+'0';
		temp[8]=' ';
		temp[9]='A';
		temp[10]='l';
		temp[11]='a';
		temp[12]='r';
		temp[13]='m';
		temp[14]=':';
		if(alarm.run)temp[15]='Y';
		else temp[15]='N';
		LCD1602_Show(0,1,temp,16);
	}
	RefreshCursor();
	LCD1602_ShowCursor();	
}
void KeyAction(uint8 keyc,uint8 statu)	//当按键被触发，自动调用（keyc：按键码，statu：按键状态，0-按键按下，1-按键弹起，2-按键长按）
{
	uint8 num;
	if(statu==1)return ;
	if(keyc>=0x30&&keyc<=0x39&&statu==0)
	{
		num=keyc-0x30;
		switch(TimeSetIndex)
		{
			case 0:return ;break;
			case 1:time.year=time.year%10+num*10;break;
			case 2:time.year=time.year/10*10+num;break;
			case 3:if(num<=1)time.month=time.month%10+num*10;if(time.month>12)time.month=num*10;break;
			case 4:if((time.month/10==0&&num!=0)||num<=2)time.month=time.month/10*10+num;break;
			case 5:if(num<=3)time.day=time.day%10+num*10;if(time.day>31)time.day=num*10;break;
			case 6:if((time.day/10<=2||num<=1)&&(time.day/10!=0||num!=0))time.day=time.day/10*10+num;break;
			case 7:if(num<=7&&num!=0)time.week=num;break;
			case 8:if(num<=2)time.hour=time.hour%10+num*10;if(time.hour>23)time.hour=num*10;break;
			case 9:if(time.hour/10<=1||num<=3)time.hour=time.hour/10*10+num;break;
			case 10:if(num<=5)time.min=time.min%10+num*10;break;
			case 11:time.min=time.min/10*10+num;break;
			case 12:if(num<=5)time.sec=time.sec%10+num*10;break;
			case 13:time.sec=time.sec/10*10+num;break;

			case 14:if(num<=2)alarm.hour=alarm.hour%10+num*10;if(alarm.hour>23)alarm.hour=num*10;break;
			case 15:if(alarm.hour/10<=1||num<=3)alarm.hour=alarm.hour/10*10+num;break;
			case 16:if(num<=5)alarm.min=alarm.min%10+num*10;break;
			case 17:alarm.min=alarm.min/10*10+num;break;
			case 18:if(num<=5)alarm.sec=alarm.sec%10+num*10;break;
			case 19:alarm.sec=alarm.sec/10*10+num;break;
			case 20:if(num==0||num==1)alarm.run=num;
		}
		if(time.month==0)time.month=1;
		if(time.day==0)time.day=1;
		DS1302_Set();
		SetTimeMainIndex(TimeSetMainIndex);
		switch(TimeSetMainIndex)
		{
			case 1:if(TimeSetIndex<7)TimeSetIndex++;else TimeSetIndex=1;break;
			case 2:if(TimeSetIndex<13)TimeSetIndex++;else TimeSetIndex=8;break;
			case 3:if(TimeSetIndex<20)TimeSetIndex++;else TimeSetIndex=14;break;
		}		
		RefreshCursor();		
		LCD1602_ShowCursor();
	}
	else if(keyc==0x26)	//up
	{
		switch(TimeSetMainIndex)
		{
			case 0:break;
			case 1:break;
			case 2:SetTimeMainIndex(1);TimeSetIndex=1;break;
			case 3:SetTimeMainIndex(2);TimeSetIndex=8;break;
		}	
	}
	else if(keyc==0x28)	//down
	{
		switch(TimeSetMainIndex)
		{
			case 0:break;
			case 1:SetTimeMainIndex(2);TimeSetIndex=8;break;
			case 2:SetTimeMainIndex(3);TimeSetIndex=14;;break;
			case 3:break;
		}		
	}
	else if(keyc==0x25)	//left
		{
			switch(TimeSetMainIndex)
			{
				case 1:if(TimeSetIndex>1)TimeSetIndex--;else TimeSetIndex=7;break;
				case 2:if(TimeSetIndex>8)TimeSetIndex--;else TimeSetIndex=13;break;
				case 3:if(TimeSetIndex>14)TimeSetIndex--;else TimeSetIndex=20;break;
			}
			RefreshCursor();
		}
	else if(keyc==0x27)	//right
		{
			switch(TimeSetMainIndex)
			{
				case 1:if(TimeSetIndex<7)TimeSetIndex++;else TimeSetIndex=1;break;
				case 2:if(TimeSetIndex<13)TimeSetIndex++;else TimeSetIndex=8;break;
				case 3:if(TimeSetIndex<20)TimeSetIndex++;else TimeSetIndex=14;break;
			}
			RefreshCursor();
		}
	else if(keyc==0x0d)	//enter
		{
			if(TimeSetMainIndex==0)
				{
					TimeSetIndex=1;
					SetTimeMainIndex(1);
				}
			else if(TimeSetMainIndex==1)
				{
					TimeSetIndex=8;
					SetTimeMainIndex(2);
				}
			else if(TimeSetMainIndex==2)
				{
					TimeSetIndex=14;
					SetTimeMainIndex(3);	
				}
			else 
				{
					ExitTimeSet(1);
				}	
		}
	else if(keyc==0x1b)	//esc
		{
			ExitTimeSet(0);
			BUZZ_Run=0;
		}
}
void Timer2_Init(uint8 ms)
{
	uint16 temp;
	temp=65536-ms*110592/120;
	RCAP2H=(uint8)(temp>>8);
	RCAP2L=(uint8)temp;
	TH2=RCAP2H;
	TL2=RCAP2L;
	PT2=1;
	T2CON=0x00;
	ET2=1;
	TR2=1;
}
void Timer0_Init(uint8 ms)
{
	uint16 temp;
	temp=65536-ms*110592/120;
	temp+=12;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)temp;
	TH0=T0H;
	TL0=T0L;
	//PT0=1;
	ET0=1;
	TMOD&=0XF0;
	TMOD|=0X01;
	TR0=1;
}
void Interrupt_Timer2() interrupt 5
{
	static uint8 ms200=0,s2=0,i=0;
	if(BUZZ_Run)BUZZ=~BUZZ;	
	TF2=0;
	if(LED_Refresh)
		if(i<8)
		{
			LCD1602_E=0;
			P0=0XFF;
			ENLED=0;
			ADDR3=1;
			P1&=0XF8;
			P1|=i;
			P0=LedBuff[0][i];
			i++;
		}
		else if(i<16)
		{
			LCD1602_E=0;
			P0=0XFF;
			ENLED=0;
			ADDR3=0;
			P1&=0XF8;
			P1|=(i-8);
			P0=LedBuff[1][i-8];
			i++;	
		}
		else i=0;
	ms200++;
	if(ms200>=200)
	{
		ms200-=200;
		Flag_200ms=1;
		s2++;
		if(s2>=10)
		{
			Flag_2s=1;
			s2-=10;
		}
	}	
}
void Interrupt_Timer0() interrupt 1
{
	TH0=T0H;
	TL0=T0L;
	KeyScan(1);
}