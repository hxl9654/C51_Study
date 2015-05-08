
#include <reg52.h>
#include <intrins.h>

sbit IO_18B20 = P0^1;  //DS18B20通信引脚

/* 软件延时函数，延时时间(t*10)us */
void DelayX10us(unsigned char t)
{
    do {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    } while (--t);
}
bit Get18B20Ack()
{
	bit ack;

	EA = 0;
	IO_18B20 = 0;
	DelayX10us(50);
	IO_18B20 = 1;
	DelayX10us(6);
	ack = IO_18B20;
	while(!IO_18B20);
	EA = 1;

	return ack;
}

void  Write18B20(unsigned char dat)
{
	unsigned char mask;
	EA = 0;
	for(mask=0x01; mask!= 0; mask<<=1)
	{
		IO_18B20 = 0;
		_nop_();
		_nop_();
		if((mask&dat) == 0)
			IO_18B20 = 0;
		else
			IO_18B20 = 1;
		DelayX10us(6);
		IO_18B20 = 1;
	}
	EA = 1;
}

unsigned char Read18B20()
{
	unsigned char dat;
	unsigned char mask;

	EA = 0;
	for(mask=0x01; mask!=0; mask<<=1)
	{
		IO_18B20 = 0;
		_nop_();
		_nop_();
		IO_18B20 = 1;
		_nop_();
		_nop_();
		if(!IO_18B20)
			dat &= ~mask;
		else
			dat |= mask;
		DelayX10us(6);
	}
	EA = 1;

	return dat;
}

bit Start18B20()
{
	bit ack;

	ack = Get18B20Ack();
	if(ack == 0)
	{
		Write18B20(0xCC);
		Write18B20(0x44);
	}
	return ~ack;
}

bit Get18B20Temp(int *temp)
{
	bit ack;
	unsigned char LSB, MSB;

	ack = Get18B20Ack();
	if(ack == 0)
	{
		Write18B20(0xCC);
		Write18B20(0xBE);
		LSB = Read18B20();
		MSB = Read18B20();
		*temp = ((int)MSB<<8) + LSB;
	}
	return ~ack;
}