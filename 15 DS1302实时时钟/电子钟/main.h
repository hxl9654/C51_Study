#ifndef MAINH
#define MAINH
typedef unsigned char uint8;
typedef unsigned int uint16;

extern void KeyScan();	//在定时器中断中调用，读取按键信息
extern void KeyDriver();	//在主循环中调用，触发按键操作

extern void LCD1602_Clear();    //LCD1602清屏
extern void LCD1602_AreaClear(uint8 x,uint8 y,uint8 len);   //LCD1602范围清屏（x：横坐标，y：纵坐标，len：清屏的字符数）
extern void LCD1602_Show(uint8 x,uint8 y,uint8 *dat,uint8 len); //LCD1602显示字符串（x：横坐标，y：纵坐标，dat：要显示的字符数组，len：要显示的长度）
extern void LCD1602_Init(); //LCD1602初始化函数，在主函数开头调用。
extern void LCD1602_CloseCursor();	//关闭LCD1602光标
extern void LCD1602_ShowCursor();	//打开LCD1602光标
extern void LCD1602_SetLocation(uint8 x,uint8 y);	//设置LCD1602光标位置

extern void DS1302_Init(uint8 *dat);  //DS1302初始化函数，在主函数开头调用，dat是设置的时间
extern uint8 DS1302_SingleRead(uint8 addr); //DS1302单字节读取，dat为读取数据存储位置，addr为寄存器地址，详见数据手册
extern void DS1302_SingleWrite(uint8 addr,uint8 dat);   //DS1302单字节写入，dat为写入数据，addr为寄存器地址，详见数据手册
extern void DS1302_BrustWrite(uint8 *dat);  //DS1302Brust模式写入，dat为uint8型的拥有8个元素的数组
extern void DS1302_BrustRead(uint8 *dat);   //DS1302Brust模式读取，dat为uint8型的拥有8个元素的数组

void Timer0_Init(uint8 ms);
void DS1302_Set();
void ToBCD(uint8 *dat);
void ToUint8(uint8 *dat);
void TimeRefresh();
void LCD1602_Refresh();
void ExitTimeSet(bit flag);
void RefreshCursor();

#endif