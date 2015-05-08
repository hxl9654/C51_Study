#include<reg51.h>
sbit KEYIN1=P2^4;
sbit KEYIN2=P2^5;
sbit KEYIN3=P2^6;
sbit KEYIN4=P2^7;
sbit KEYOUT1=P2^3;
sbit KEYOUT2=P2^2;
sbit KEYOUT3=P2^1;
sbit KEYOUT4=P2^0;
sbit BUZZ=P1^6;
typedef unsigned char uint8;
typedef unsigned int uint16;
bit flag1s=0,run=0,buzz=0;
uint16 time=0;
unsigned char code ledchar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
unsigned char code KeyCodeMap[4][4] = { //矩阵按键编号到标准键盘键码的映射表
    { 0x31, 0x32, 0x33, 0x26 }, //数字键1、数字键2、数字键3、向上键
    { 0x34, 0x35, 0x36, 0x25 }, //数字键4、数字键5、数字键6、向左键
    { 0x37, 0x38, 0x39, 0x28 }, //数字键7、数字键8、数字键9、向下键
    { 0x30, 0x1B, 0x0D, 0x27 }	//数字键0、ESC键、  回车键、 向右键
	};
uint16 pdata keytime[4][4]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16 pdata keylimit[4][4]={
	{1000,1000,1000,1000},
	{1000,1000,1000,1000},
	{1000,1000,1000,1000},
	{1000,1000,1000,1000}
	};
uint8 pdata key[4][4]={
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff}
	};
uint8 pdata key1[4][4]={
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff}
	};
uint8 data key2[4][4]={
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff}
	};
uint8 T0H,T0L;
uint8 ledbuff[7]={0xff,0xff,0xff,0xff,0xff,0xff,0xff};

void timer0set(uint16 t)
{
	uint16 temp;
	temp=t*110592/120;
	temp=65536-temp;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)temp;
	TH0=T0H;
	TL0=T0L;
	TR0=1;
} 
void setled()
{
	uint8 i;
	uint16 time1;
	time1=time;
	for(i=0;i<6;i++)
	{
		ledbuff[i]=ledchar[time1%10];
		time1=time1/10;
	}
}
void keyaction(uint8 kcode)
{
	if(kcode==0x26&&run==0)	  //上
		{
			time++;
			setled();
		}
	else if(kcode==0x28&&run==0)	  //下
		{
			if(time>1)time--;
			setled();
		}
	else if(kcode==0x1b)	  //esc
		{
			time=0;
			run=0;
			buzz=0;
			flag1s=0;
			ledbuff[6]=0xff;
			setled();
		}
	else if(kcode==0x0d&&run==0&&time!=0)	  //回车
		{
			buzz=0;
			flag1s=0;
			ledbuff[6]=0xff;
			setled();
			run=1;
		}
} 
void main()
{
	uint8 i,j;
	TMOD=0X01;
	timer0set(1);
	setled();
	EA=1;
	ET0=1;
	P1&=0XE8;
	while(1)
	{
		if(flag1s&&run)	
		{
			flag1s=0;
			time--;
			setled();
			if(time==0)
			{
				buzz=1;
				run=0;
				ledbuff[6]=0x00;
			}
		}
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
			{
				if(key[i][j]==0x00)
				{
					key[i][j]=0xff;
					keyaction(KeyCodeMap[i][j]);
				}
				else if(key[i][j]==0x01)
				{
					key[i][j]=0xff;
				}
			}
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				if(keytime[i][j]>=1000)
					{
						if(keytime[i][j]>=keylimit[i][j])
						{
							keyaction(KeyCodeMap[i][j]);
							keylimit[i][j]+=200;
						}
						else ;
					}
				else keylimit[i][j]=1000;			
	}
}
void it0() interrupt 1
{
	static uint16 f1s=0;
	uint8 k;
	static uint8 i=0,j=0;
	TH0=T0H;
	TL0=T0L;
	if(buzz)BUZZ=~BUZZ;
	if(run)
	{
		f1s++;
		if(f1s>=1000)
			{
				f1s-=1000;
				flag1s=1;
			}	
	}
	key2[i][0]=(key2[i][0]<<1)|KEYIN1;
	key2[i][1]=(key2[i][1]<<1)|KEYIN2;
	key2[i][2]=(key2[i][2]<<1)|KEYIN3;
	key2[i][3]=(key2[i][3]<<1)|KEYIN4;
	for(k=0;k<4;k++)
	{
		if((key2[i][k]&0x0f)==0x00)
			{
				if(key1[i][k]==0x01){key1[i][k]=0x00;key[i][k]=0x00;keytime[i][k]=0;}
				else keytime[i][k]+=4;
			}
		else if((key2[i][k]&0x0f)==0x0f)
			{
				if(key1[i][k]==0x00){key1[i][k]=0x01;key[i][k]=0x01;keytime[i][k]=0;}
				else keytime[i][k]+=4;
			}
		else keytime[i][k]=0;
	}
	P0=0XFF;
	P1&=0XF8;			   
	P1|=j;
	P0=ledbuff[j];
	j++;
	if(j>=7)j=0;
	i++;
	if(i>=4)i=0;
	switch(i)
	{
		case 0:	KEYOUT4=1;KEYOUT1=0;break;
		case 1:	KEYOUT1=1;KEYOUT2=0;break;
		case 2:	KEYOUT2=1;KEYOUT3=0;break;
		case 3:	KEYOUT3=1;KEYOUT4=0;break;
	}
}