#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;
sbit KEYOUT4=P2^0;
sbit KEYOUT3=P2^1;
sbit KEYOUT2=P2^2;
sbit KEYOUT1=P2^3;
sbit KEYIN1=P2^4;
sbit KEYIN2=P2^5;
sbit KEYIN3=P2^6;
sbit KEYIN4=P2^7;
uint8 p=0,p1=0;
unsigned char code LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
uint8 num[6]={0x00,0x00,0x00,0x00,0x00,0x00};
uint8 key[4][4]={{0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff}
				};
uint8 key1[4][4]={{0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff}
				};
uint8 key2[4][4]={{0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff}
				};
void refreshled()
{
	 uint8 i,j;
	 for(i=0;i<4;i++)
	 	for(j=0;j<4;j++)
			if(key2[i][j]==0)
				{
					key2[i][j]=0xff;
					num[0]=i*4+j;
				}

	 P0=0Xff;
	 switch(p)
	 {
	 	case 0:ADDR0=0;ADDR2=0;P0=LedChar[num[p]];break;
		case 1:ADDR0=1;P0=LedChar[num[p]];break;
		case 2:ADDR0=0;ADDR1=1;P0=LedChar[num[p]];break;
		case 3:ADDR0=1;P0=LedChar[num[p]];break;
		case 4:ADDR0=0;ADDR1=0;ADDR2=1;P0=LedChar[num[p]];break;
		case 5:ADDR0=1;P0=LedChar[num[p]];break;
	 }
	 p++;
	 if(p>=6)p=0;
}
void main()
{
	ADDR3=1;
	ENLED=0;
	ADDR1=0;
	TH0=0XFC;
	TL0=0X67;
	TMOD=0X01;
	KEYOUT1=0;
	EA=1;
	ET0=1;
	TR0=1;
	while(1)
	{
		refreshled();
	}
}
void scan() interrupt 1
{
	TH0=0XFC;
	TL0=0X67;
	key[p1][0]=(key[p1][0]<<1)|KEYIN1;
	key[p1][1]=(key[p1][1]<<1)|KEYIN2;
	key[p1][2]=(key[p1][2]<<1)|KEYIN3;
	key[p1][3]=(key[p1][3]<<1)|KEYIN4;
	if((key[p1][0]&0x0f)==0x00)
	{
		if(key[p1][0])
			key2[p1][0]=0;
		key[p1][0]=0x00;
	}
	else if((key[p1][0]&0x0f)==0x0f)
	{
		if(!key[p1][0])
			key2[p1][0]=1;
		key[p1][0]=0xff;
	}

	if((key[p1][1]&0x0f)==0x00)
	{
		if(key[p1][1])
			key2[p1][1]=0;
		key[p1][1]=0x00;
	}
	else if((key[p1][1]&0x0f)==0x0f)
	{
		if(!key[p1][1])
			key2[p1][1]=1;
		key[p1][1]=0xff;
	}
	
	if((key[p1][2]&0x0f)==0x00)
	{
		if(key[p1][2])
			key2[p1][2]=0;
		key[p1][2]=0x00;
	}
	else if((key[p1][2]&0x0f)==0x0f)
	{
		if(!key[p1][2])
			key2[p1][2]=1;
		key[p1][2]=0xff;
	}
	
	if((key[p1][3]&0x0f)==0x00)
	{
		if(key[p1][3])
			key2[p1][3]=0;
		key[p1][3]=0x00;
	}
	else if((key[p1][3]&0x0f)==0x0f)
	{
		if(!key[p1][3])
			key2[p1][3]=1;
		key[p1][3]=0xff;
	}
	switch(p1)
	{
		case 0: KEYOUT4=1;KEYOUT1=0;break;
		case 1: KEYOUT1=1;KEYOUT2=0;break;
		case 2: KEYOUT2=1;KEYOUT3=0;break;
		case 3: KEYOUT3=1;KEYOUT4=0;break;
	}
	p1++;
	if(p1>=4)p1=0;
}