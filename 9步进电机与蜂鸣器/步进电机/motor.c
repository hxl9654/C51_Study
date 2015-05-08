#include<reg51.h>
typedef char int8;
typedef unsigned char uint8;
typedef int int16;
typedef long int32;
sbit KEYOUT4=P2^0;
sbit KEYOUT3=P2^1;
sbit KEYOUT2=P2^2;
sbit KEYOUT1=P2^3;
sbit KEYIN1=P2^4;
sbit KEYIN2=P2^5;
sbit KEYIN3=P2^6;
sbit KEYIN4=P2^7;
uint8 key[4][4]={{0x01,0x01,0x01,0x01},
				 {0x01,0x01,0x01,0x01},
				 {0x01,0x01,0x01,0x01},
				 {0x01,0x01,0x01,0x01}
				};
uint8 key2[4][4]={{0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff}
				};
uint8 code keycode[4][4]={ //矩阵按键编号到标准键盘键码的映射表
	{ 0x31, 0x32, 0x33, 0x26 }, //数字键1、数字键2、数字键3、向上键 
	{ 0x34, 0x35, 0x36, 0x25 }, //数字键4、数字键5、数字键6、向左键     
    { 0x37, 0x38, 0x39, 0x28 },  //数字键7、数字键8、数字键9、向下键
	{ 0x30, 0x1B, 0x0D, 0x27 } //数字键0、ESC键、  回车键、 向右键	
};
uint8 keystatu[4][4]={{0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff},
				 {0xff,0xff,0xff,0xff}
				};
uint8 code motorcode[8]={0x0E, 0x0C, 0x0D, 0x09, 0x0B, 0x03, 0x07, 0x06};
int32 step=0;
void motorstop()
{
	step=0;
	P1|=0x0f;
}
void motormove()
{
	 static uint8 i=0;
	 if(step==0){motorstop();return ;}
	 if(step>0)
	 {
	 step--;
	 P1&=0XF0;
	 P1|=motorcode[i];
	 i++;
	 i&=0x07;
	 }
	 else if(step<0)
	 {
	 if(i==0)i=8;
	 step++;
	 P1&=0XF0;
	 P1|=motorcode[i-1];
	 i--;
	 }
}
void motor_set(int32 n,bit s)
{
	EA=0;
	step=n*4076/360;
	if(s==1)step=-step;
	EA=1;
}
void keydo(uint8 keyc)
{
	static bit t=0;
	if(keyc<=0x39&&keyc>=0x30)
	{
		if(t==0)
			motor_set((keyc-'0')*360,0);
		else  motor_set((keyc-'0')*360,1);
	}
	if(keyc==0x1b)
	{
		motorstop();
	}
	if(keyc==0x25)
	{
		t=1;
	}
	if(keyc==0x27)
	{
		t=0;
	}
	if(keyc==0x26)
	{
		motor_set(90,0);
	}
	if(keyc==0x28)
	{
		motor_set(90,1);
	}
	if(keyc==0x0D)
	{
		motor_set(99999999,t);
	}
}
void key_do()
{
	uint8 i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(keystatu[i][j]==0x00)
				 keystatu[i][j]=0xff;
			else if(keystatu[i][j]==0x01)
				 {
				 	 keystatu[i][j]=0xff;
					 keydo(keycode[i][j]);
				 }
}
void main()
{
	TMOD = 0x01;	//设置定时器模式
	TH0 = 0xFC;		//设置定时初值
	TL0 = 0x67;		//设置定时初值
	EA=1;
	ET0=1;
	TR0=1;
	while(1)
	{
		key_do();
	}	  
}
void timer0() interrupt 1
{
	static bit e=0;
	static uint8 i=0;
	TH0 = 0xFC;		//设置定时初值
	TL0 = 0x67;
	if(e==0)e=1;
	else
	{
		e=0;
		motormove();
	}
	key2[i][0]=(key2[i][0]<<1)|KEYIN1;
	key2[i][1]=(key2[i][1]<<1)|KEYIN2;
	key2[i][2]=(key2[i][2]<<1)|KEYIN3;
	key2[i][3]=(key2[i][3]<<1)|KEYIN4;
	P0=key2[0][0];
	if((key2[i][0]&0x0f)==0x0f)
	{
		if(key[i][0]==0)keystatu[i][0]=0X01;
		key[i][0]=1;
	}
	else if((key2[i][0]&0x0f)==0x00)
	{
		if(key[i][0]==1)keystatu[i][0]=0X00;
		key[i][0]=0;
	}

	if((key2[i][1]&0x0f)==0x0f)
	{
		if(key[i][1]==0)keystatu[i][1]=0X01;
		key[i][1]=1;
	}
	else if((key2[i][1]&0x0f)==0x00)
	{
		if(key[i][1]==1)keystatu[i][1]=0X00;
		key[i][1]=0;
	}

	if((key2[i][2]&0x0f)==0x0f)
	{
		if(key[i][2]==0)keystatu[i][2]=0X01;
		key[i][2]=1;
	}
	else if((key2[i][2]&0x0f)==0x00)
	{
		if(key[i][2]==1)keystatu[i][2]=0X00;
		key[i][2]=0;
	}

	if((key2[i][3]&0x0f)==0x0f)
	{
		if(key[i][3]==0)keystatu[i][3]=0X01;
		key[i][3]=1;
	}
	else if((key2[i][3]&0x0f)==0x00)
	{
		if(key[i][3]==1)keystatu[i][3]=0X00;
		key[i][3]=0;
	}
	i++;
	i&=0x03;
	switch(i)
	{
		case 0:KEYOUT4=1;KEYOUT1=0;break;
		case 1:KEYOUT1=1;KEYOUT2=0;break;
		case 2:KEYOUT2=1;KEYOUT3=0;break;
		case 3:KEYOUT3=1;KEYOUT4=0;break;
	}
}
