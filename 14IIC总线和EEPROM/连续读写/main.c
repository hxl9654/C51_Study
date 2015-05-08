#include<reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

extern void LCD1602_Show(uint8 x,uint8 y,uint8 *dat,uint8 len);
extern void LCD1602_Init();
extern void LCD1602_Clear();
extern void EEPROM_ReadString(uint8 addr,uint8 len,uint8 *dat);
extern void EEPROM_WriteString(uint8 addr,uint8 *dat,uint8 len);
extern void EEPROM_Write(uint8 addr,uint8 dat);
void Uint8ToString(uint8 *dat,uint8 *show)
{
	uint8 i,temp;
	for(i=0;i<5;i++)
		{
			temp=*dat;
			temp>>=4;
			if(temp<10)temp+='0';
			else temp=temp-10+'A';
			*show=temp;
			show++;

			temp=*dat&0x0F;
			if(temp<10)temp+='0';
			else temp=temp-10+'A';
			*show=temp;
			show++;
			*show=' ';
			show++;
			dat++;	
		}
	*show=' ';
}
void main()
{
	uint8 dat[5],show[16],i;
		
	EEPROM_ReadString(0x8E,5,dat);
	Uint8ToString(dat,show);	

	for(i=0;i<5;i++)
		{
		 	dat[i]+=i;
			//EEPROM_Write(0x88+i,dat[i]);
		}
	
	EEPROM_WriteString(0x8E,dat,5);
	LCD1602_Init();
	LCD1602_Show(0,0,show,16);
	while(1);
}