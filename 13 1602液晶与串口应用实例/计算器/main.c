#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit KEYOUT1=P2^3;
sbit KEYOUT2=P2^2;
sbit KEYOUT3=P2^1;
sbit KEYOUT4=P2^0;
sbit KEYIN1=P2^4;
sbit KEYIN2=P2^5;
sbit KEYIN3=P2^6;
sbit KEYIN4=P2^7;
sbit AA=P3^7;
extern lcdshow(uint8 x,uint8 y,uint8 *str,uint8 n);
extern clear();
extern lcdinit();
uint8 T0H,T0L;
long num1=0,num2=0,ans=0;
uint8 action=5;
uint8 xdata key[4][4]={
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff}};
uint8 xdata key1[4][4]={
	{0x01,0x01,0x01,0x01},
	{0x01,0x01,0x01,0x01},
	{0x01,0x01,0x01,0x01},
	{0x01,0x01,0x01,0x01}
	};
uint8 key2[4][4]={
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff}};
uint8 keycode[4][4]={ //矩阵按键编号到标准键盘键码的映射表
	{ 0x31, 0x32, 0x33, 0x26 }, //数字键1、数字键2、数字键3、向上键
    { 0x34, 0x35, 0x36, 0x25 }, //数字键4、数字键5、数字键6、向左键
    { 0x37, 0x38, 0x39, 0x28 },  //数字键7、数字键8、数字键9、向下键
    { 0x30, 0x1B, 0x0D, 0x27 } //数字键0、ESC键、  回车键、 向右键
};
void timer0init()
{
	TMOD=0X01;
	EA=1;
	ET0=1;
	T0H=0xFC;
	T0L=0x67;
	TH0=T0H;
	TL0=T0L;
	TR0=1;
}
void reset()
{
	 clear();
	 num1=0;
	 num2=0;
	 ans=0;
	 action=5;
}
void show(uint8 n,long num)
{
	uint8 i,j;
	uint8 a[20],b[20];
	uint8 flag=1;
	AA=0;
	if(num<0){num=-num;flag=0;}
	for(i=0;num!=0;i++)
		{
			a[i]=num%10+'0';
			num/=10;	
		}
	AA=1;
	for(j=1;j<=i;j++)
		{
			b[16-j]=a[j-1];
		}
	for(;j<=16;j++)
		b[16-j]=' ';
	if(flag==0)b[16-j]='-';
	if(n==1)
		switch(action)
			{
				case 0:b[0]='+';break;
				case 1:b[0]='-';break;
				case 2:b[0]='*';break;
				case 3:b[0]='/';break;
				case 4:b[0]='=';break;
			}
	lcdshow(0,n,b,16);	
}
void keynumaction(uint8 num)
{
	if(action==5||action==4)
		{
			num1*=10;
			num1+=num;
			show(0,num1);
		}
	else {
			num2*=10;
			num2+=num;
			show(1,num2);
		}
}
void slove()
{
	switch(action)
		{
			case 0:ans=num1+num2;break;
			case 1:ans=num1-num2;break;
			case 2:ans=num1*num2;break;
			case 3:ans=num1/num2;break;
		}
	clear();
	action=4;
	show(1,ans);
	num1=0;
	num2=0;
	action=5;	
}
void keydoaction(uint8 keyc)
{
	uint8 b[2];
	action=keyc;
	switch(action)
		{
			case 0:b[0]='+';break;
			case 1:b[0]='-';break;
			case 2:b[0]='*';break;
			case 3:b[0]='/';break;
		}
	lcdshow(0,1,b,1);
}
void keyaction(uint8 keyc)
{
	if(keyc>=0x30&&keyc<=0x39)
		{
			keynumaction(keyc-0x30);
		}
	else if(keyc==0x1b)
		{
			reset();
		}
	else if(keyc==0x0d)
		{
			slove();
		}
	else if(keyc==0x25)
	 	{
			keydoaction(2);
		}
	else if(keyc==0x26)
	 	{
			keydoaction(0);
		}
	else if(keyc==0x27)
	 	{
			keydoaction(3);
		}
 	else if(keyc==0x28)
	 	{
			keydoaction(1);
		}
}
void main()
{
	uint8 i,j;
	lcdinit();
	timer0init();
	lcdinit();	
	while(1)
	{
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				if(key[i][j]!=0xff)
					{
						if(key[i][j]==0x00)keyaction(keycode[i][j]);
						key[i][j]=0xff;
					}	
	}
}
void it0() interrupt 1
{
	static uint8 i=0,j=0;
	TH0=T0H;
	TL0=T0L;
	key2[i][0]=(key2[i][0]<<1)|KEYIN1;
	key2[i][1]=(key2[i][1]<<1)|KEYIN2;
	key2[i][2]=(key2[i][2]<<1)|KEYIN3;
	key2[i][3]=(key2[i][3]<<1)|KEYIN4;

	for(j=0;j<4;j++)
	{
		if((key2[i][j]&0x0f)==0x0f)
			{
				if(key1[i][j]==0x00)key[i][j]=0x01;
				key1[i][j]=0x01;
			}
		else if((key2[i][j]&0x0f)==0x00)
			{
				if(key1[i][j]==0x01)key[i][j]=0x00;
				key1[i][j]=0x00;
			}
	}
	i++;
	if(i>=4)i=0;
	switch(i)
	{
		case 0:KEYOUT4=1;KEYOUT1=0;break;
		case 1:KEYOUT1=1;KEYOUT2=0;break;
		case 2:KEYOUT2=1;KEYOUT3=0;break;
		case 3:KEYOUT3=1;KEYOUT4=0;break;
	}
}
