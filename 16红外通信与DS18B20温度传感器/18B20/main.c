#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
extern void LCD1602_Clear();    //LCD1602清屏
extern void LCD1602_Show(uint8 x,uint8 y,uint8 *dat,uint8 len); //LCD1602显示字符串（x：横坐标，y：纵坐标，dat：要显示的字符数组，len：要显示的长度）
extern void LCD1602_Init(); //LCD1602初始化函数，在主函数开头调用。
extern void DS18B20_Start(uint8 *addr);
extern int DS18B20_GetTemp(uint8 *addr);
extern bit DS18B20_Init();
uint8 addr=0x00;
void main()
{
	int tem;
	uint16 i;
	uint8 temp;
	uint8 LCDBuff[10];

	LCD1602_Init();
	DS18B20_Start(&addr);
	LCD1602_Show(0,0,"Wait..",6);
	for(i=0;i<33000;i++);
	while(1)
	{
		if(DS18B20_Init())
		{
			tem=DS18B20_GetTemp(&addr);
			if(tem<0)
			{
				tem=-tem;
				LCDBuff[0]='-';
			}		
			else LCDBuff[0]=' ';
			temp=(uint8)(tem>>4);
			LCDBuff[1]=temp/100+'0';			
			LCDBuff[2]=temp/10%10+'0';
			LCDBuff[3]=temp%10+'0';
			LCDBuff[4]='.';
			temp=tem&0x0F;
			temp=temp*10/16;
			LCDBuff[5]=temp+'0';
			LCDBuff[6]='\0';
			LCD1602_Show(0,0,LCDBuff,6);
		}
		else LCD1602_Show(0,0,"error!",6);
		DS18B20_Start(&addr);	    
		for(i=0;i<35000;i++);
	}				
}