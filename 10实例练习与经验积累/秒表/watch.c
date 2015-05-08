#include<reg51.h>
unsigned char code ledchar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};
typedef unsigned char uint8;
typedef unsigned int uint16;
uint8 ledbuff[6],T0H,T0L;
uint16 intpart=0;
uint8 floatpart=0,key1=0xff,key2=0xff;
uint8 key1l=0x01,key2l=0x01;
bit ledrefresh=0,run=0;
sbit keyin2=P2^5;
sbit keyin3=P2^6;

void Timer0Conf(uint8 ms)
{
	uint16 tem;
	tem=ms*110592/120;
	tem=65526-tem+18;
	T0H=(uint8)(tem>>8);
	T0L=(uint8)(tem);
	TH0=T0H;
	TL0=T0L;
	TMOD&=0XF0;
	TMOD|=0X01;
	EA=1;
	ET0=1;
	TR0=1;
}
void ledrefreshf()
{
	int i;
	uint8 temp[3];
	ledrefresh=0;
	ledbuff[0]=ledchar[floatpart%10];
	ledbuff[1]=ledchar[floatpart/10];
	ledbuff[2]=ledchar[intpart%10]&0x7F;
	temp[0]=ledchar[intpart/10%10];
	temp[1]=ledchar[intpart/100%10];
	temp[2]=ledchar[intpart/1000];		
	for(i=5;i>2;i--)
		if(temp[i-3]==ledchar[0])ledbuff[i]=0xff;
		else break;
	for(;i>2;i--)
		ledbuff[i]=temp[i-3];
}
void stop()
{
	intpart=0;
	floatpart=0;
	run=0;
	ledrefreshf();
}
void key2f()
{
	if(run)run=0;
	else run=1;
}
void keydriver()
{
	if(key1==0x01)
	{
		key1=0xff;
		stop();
	}
	else if(key1==0x00)key1=0xff;
	if(key2==0x01)
	{
		key2=0xff;
		key2f();
	}
	else if(key2==0x00)key2=0xff;
}
void main()
{
	Timer0Conf(2);
	P2&=0XFE;
	P1&=0XEF;
	ledrefresh=1;
	while(1)
	{
		if(ledrefresh)ledrefreshf();
		keydriver();
	}
}
void led()
{	
	static uint8 i=0;
	P0=0XFF;
	P1&=0XF8;
	P1|=i;
	P0=ledbuff[i];
	i++;
	if(i>5)i=0;
}
void timer0int() interrupt 1
{
	static uint8 key[2]={0xff,0xff};
	static uint8 ms10=0;
	TH0=T0H;
	TL0=T0L;
	if(run)
	{
		ms10++;
		if(ms10>=5)
		{
			ms10-=5;
			floatpart++;
			if(floatpart>=100)
			{
				intpart++;
				floatpart-=100;
			}
			ledrefresh=1;
		}
	}
	led();
	key[0]=(key[0]<<1)|keyin2;
	key[1]=(key[1]<<1)|keyin3;
	if((key[0]&0x0f)==0x0f)
	{		
		if(key1l==0x00)key1=0x01;
		key1l=0x01;
	}
	else if((key[0]&0x0f)==0x00)
	{		
		if(key1l==0x01)key1=0x00;
		key1l=0x00;
	}

	if((key[1]&0x0f)==0x0f)
	{		
		if(key2l==0x00)key2=0x01;
		key2l=0x01;
	}
	else if((key[1]&0x0f)==0x00)
	{		
		if(key2l==0x01)key2=0x00;
		key2l=0x00;
	}
}