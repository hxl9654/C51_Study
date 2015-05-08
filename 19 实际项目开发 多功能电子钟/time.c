#include"main.h"

unsigned char code LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
uint8 code WeekMod[7][8] = {  //星期X字符图片表    
    {0xEF, 0xE7, 0xE3, 0xE7, 0xE7, 0xE7, 0xE7, 0xC3},  //星期1
    {0xC3, 0x81, 0x9D, 0x87, 0xC3, 0xF9, 0xC1, 0x81},  //星期2
    {0xC3, 0x81, 0x9D, 0xC7, 0xC7, 0x9D, 0x81, 0xC3},  //星期3
    {0xCF, 0xC7, 0xC3, 0xC9, 0xC9, 0x81, 0xCF, 0xCF},  //星期4
    {0x81, 0xC1, 0xF9, 0xC3, 0x87, 0x9D, 0x81, 0xC3},  //星期5
    {0xC3, 0x81, 0xF9, 0xC3, 0x81, 0x99, 0x81, 0xC3},  //星期6
	{0xFF, 0x99, 0x00, 0x00, 0x00, 0x81, 0xC3, 0xE7}  //星期日(红心)
};

extern uint8 LedBuff[2][8];
extern int wendu;
extern bit BUZZ_Run;

struct StructAlarm alarm;
struct StructTime time;
bit Flag_200ms=0,Flag_2s=0,LED_Refresh=1;
void LCD1602_Refresh()
{
	uint8 temp[16];
	int wendutemp;
	wendutemp=wendu;
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
	if(wendutemp<0){temp[11]='-';wendutemp=(-wendutemp);wendutemp>>=4;}
	else {wendutemp>>=4;temp[11]=' ';} 
	temp[12]=wendutemp/100+'0';
	if(temp[12]=='0')temp[12]=' ';
	temp[13]=wendutemp/10%10+'0';
	temp[14]=wendutemp%10+'0';
	temp[15]='\'';	
	LCD1602_Show(0,0,temp,16);

	temp[0]='A';
	temp[1]='l';
	temp[2]='a';
	temp[3]='r';
	temp[4]='m';	
	if(alarm.run)
	{
		temp[5]=' ';
		temp[6]=' ';
		temp[7]=' ';
		temp[8]=alarm.hour/10+'0';
		temp[9]=alarm.hour%10+'0';
		temp[10]=':';
		temp[11]=alarm.min/10+'0';
		temp[12]=alarm.min%10+'0';
		temp[13]=':';
		temp[14]=alarm.sec/10+'0';
		temp[15]=alarm.sec%10+'0';
	}
	else
	{
		temp[5]=' ';
		temp[6]=' ';
		temp[7]='O';
		temp[8]='F';
		temp[9]='F';
		temp[10]=' ';
		temp[11]=' ';
		temp[12]=' ';
		temp[13]=' ';
		temp[14]=' ';
		temp[15]=' ';
	}
	LCD1602_Show(0,1,temp,16);
}
void Timer_Refresh()
{	
	uint8 i;
	LedBuff[0][0]=LedChar[time.sec%10];
	LedBuff[0][1]=LedChar[time.sec/10];
	LedBuff[0][2]=LedChar[time.min%10]&0x7F;
	LedBuff[0][3]=LedChar[time.min/10];
	LedBuff[0][4]=LedChar[time.hour%10]&0x7F;
	LedBuff[0][5]=LedChar[time.hour/10];
	LedBuff[0][6]=rand();
	for(i=0;i<8;i++)
		LedBuff[1][i]=WeekMod[time.week-1][i];	
}
void TimeRefresh(bit flag)
{
	uint8 dat[8];
	DS1302_BrustRead(dat);
	if(time.hour==alarm.hour&&time.min==alarm.min&&time.sec==alarm.sec&&alarm.run==1)BUZZ_Run=1;
	ToUint8(dat);
	Timer_Refresh();
	if(flag)LCD1602_Refresh();
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