#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit BUZZ=P1^6;
extern bit sendf;
uint8 idata d[100];
extern uint8 index;
uint8 t30=0;
uint8 in;
bit buzzen=0;
bit run=0;
bit rendf=0;
bit flag=1;
uint8 flag0=0;
uint8 index1=1;
uint8 T0H,T0L;
extern void lcdshow(uint8 x,uint8 y,uint8 *str,uint8 n);
extern void lcdinit();
extern void uartinit(uint16 baud);
extern void lcdwritecmd(uint8 cmd);
char code buzzc[]="buzz ";
char code showc[]="show ";
void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void timer0init(uint8 t)
{
	uint16 temp;
	temp=65536-t*110592/120;
	T0H=(uint8)(temp>>8);
	T0L=(uint8)temp;
	TH0=T0H;
	TL0=T0L;
	TMOD&=0XF0;
	TMOD|=0X01;
	ET0=1;
	TR0=1;	
}
void main()
{
	uint8 i,index=0;
	lcdinit();
	Delay100ms();		
	lcdinit();
	uartinit(9600);
	timer0init(1);
	while(1)
	{
		if(rendf)
		{
			flag=1;
			rendf=0;
			if(d[0]=='b')
			{
				for(i=0;buzzc[i]!='\0';i++)
					if(d[i]!=buzzc[i]){flag=0;break;}
				if(flag==1)
					{
						if(d[i]=='o'&&d[i+1]=='n')buzzen=1;
						if(d[i]=='o'&&d[i+1]=='f'&&d[i+2]=='f')buzzen=0;
					}
			}
			else if(d[0]=='s')
			{
				for(i=0;showc[i]!='\0';i++)
					if(d[i]!=showc[i]){flag=0;break;}
				if(flag==1)
					{
						lcdwritecmd(0x01);
						lcdshow(0,0,d+i,in-i);
					}	
			}
			SBUF=d[0];
		}
		if(sendf)
			{
				sendf=0;
				if(d[index1]=='\0'&&flag0==0)flag0=1;				
				else if(flag0==0) 
				{
					SBUF=d[index1];
					index1++;
				}
				if(flag0==1){SBUF='\r';flag0=2;}
				else if(flag0==2){SBUF='\n';flag0=3;}
				else if(flag0==3){index1=1;flag0=0;}
			}
	}
}
void t0i() interrupt 1
{
	TH0=T0H;
	TL0=T0L;
	if(run)t30++;
	if(buzzen)BUZZ=~BUZZ;
	if(t30>=30)
	{
		d[index]='\0';
		in=index;
		index=0;
		rendf=1;
		t30=0;
		run=0;			
	}											
}