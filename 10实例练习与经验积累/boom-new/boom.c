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
uint8 T0H,T0L;
uint16 time=0,f1s=0;
unsigned char code ledchar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
unsigned char code KeyCodeMap[4][4] = { //¾ØÕó°´¼ü±àºÅµ½±ê×¼¼üÅÌ¼üÂëµÄÓ³Éä±í
    { 0x31, 0x32, 0x33, 0x26 }, //Êý×Ö¼ü1¡¢Êý×Ö¼ü2¡¢Êý×Ö¼ü3¡¢ÏòÉÏ¼ü
    { 0x34, 0x35, 0x36, 0x25 }, //Êý×Ö¼ü4¡¢Êý×Ö¼ü5¡¢Êý×Ö¼ü6¡¢Ïò×ó¼ü
    { 0x37, 0x38, 0x39, 0x28 }, //Êý×Ö¼ü7¡¢Êý×Ö¼ü8¡¢Êý×Ö¼ü9¡¢ÏòÏÂ¼ü
    { 0x30, 0x1B, 0x0D, 0x27 }	//Êý×Ö¼ü0¡¢ESC¼ü¡¢  »Ø³µ¼ü¡¢ ÏòÓÒ¼ü
	};
uint8 idata key[4][4]={
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff}};
uint8 idata key1[4][4]={
	{0x01,0x01,0x01,0x01},
	{0x01,0x01,0x01,0x01},
	{0x01,0x01,0x01,0x01},
	{0x01,0x01,0x01,0x01}};
uint8 idata key2[4][4]={
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff}};
uint8 ledbuff[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
uint16 idata keylimit[4][4]={
	{1000,1000,1000,1000},
	{1000,1000,1000,1000},
	{1000,1000,1000,1000},
	{1000,1000,1000,1000}};
uint16 idata keytime[4][4]={
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}};
void t0set(uint8 t)
{
	uint16 temp;
	temp=t*110592/120;
	temp=65536-temp;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)temp;
	TMOD=0X01;
	TH0=T0H;
	TL0=T0L;
	TR0=1;
}
void refresh()
{
	uint16 time1;
	uint8 i;
	time1=time;
	for(i=0;i<6;i++)
		{
			ledbuff[i]=ledchar[time1%10];
			time1=time1/10;
		}
}
void keyaction(uint8 keyc)
{
	if(keyc==0x26&&run==0)//up
	{
		time++;
		refresh();
	}
	else if(keyc==0x28&&run==0)//down
	{
		if(time>1)
			time--;
		refresh();
	}
	else if(keyc==0x1b)//esc
	{
		run=0;
		buzz=0;
		flag1s=0;
		f1s=0;
		time=0;
		ledbuff[6]=0xff;
		refresh();
	}
	else if(keyc==0x0d&&run==0&&time!=0)//enter
	{
		buzz=0;
		flag1s=0;
		f1s=0;
		run=1;
		refresh();
	}
}
void main()
{
	uint8 i,j;
	t0set(1);
	EA=1;
	ET0=1;
	refresh();
	while(1)
	{
		if(run&&flag1s)
		{
			flag1s=0;
			if(time==0)
			{
				run=0;
				buzz=1;
				ledbuff[6]=0x00;
			}
			else time--;
			refresh();
		}
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				{
					if(key[i][j]!=0xff)
						if(key[i][j]==0x00)
							{
								key[i][j]=0xff;
								keyaction(KeyCodeMap[i][j]);
							}
						else if(key[i][j]==0x01)
								key[i][j]=0xff;
				}
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				{
					if(keytime[i][j]>=1000)
						if(keytime[i][j]>=keylimit[i][j])
							{
								keylimit[i][j]+=200;
								keyaction(KeyCodeMap[i][j]);
							}
						else ;
					else keylimit[i][j]=1000;	
				}
	}
}
void t0i() interrupt 1
{
	static uint8 i=0,j=0;
	uint8 k;
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
	P0=0XFF;
	P1&=0XE8;
	P1|=i;
	P0=ledbuff[i];
	i++;
	if(i>=7)i=0;
	key2[j][0]=(key2[j][0]<<8)|KEYIN1;
	key2[j][1]=(key2[j][1]<<8)|KEYIN2;
	key2[j][2]=(key2[j][2]<<8)|KEYIN3;
	key2[j][3]=(key2[j][3]<<8)|KEYIN4;

	for(k=0;k<4;k++)
	{
		if((key2[j][k]&0x0f)==0x0f)
			{
				if(key1[j][k]==0x00)
					{
						key[j][k]=0x01;
						key1[j][k]=0x01;
						keytime[j][k]=0;
					}
				else keytime[j][k]+=4;
			}
		else if((key2[j][k]&0x0f)==0x00)
			{
				if(key1[j][k]==0x01)
					{
						key[j][k]=0x00;
						key1[j][k]=0x00;
						keytime[j][k]=0;
					}
				else keytime[j][k]+=4;
			}

	}

	j++;
	if(j>=4)j=0;

	switch(j)
	{
		case 0:KEYOUT4=1;KEYOUT1=0;break;
		case 1:KEYOUT1=1;KEYOUT2=0;break;
		case 2:KEYOUT2=1;KEYOUT3=0;break;
		case 3:KEYOUT3=1;KEYOUT4=0;break;
	}

}