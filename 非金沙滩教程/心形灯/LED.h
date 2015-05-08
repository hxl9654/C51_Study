typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;
#define LED15 {0X80,0,0,0}
#define LED14 {0X40,0,0,0}
#define LED13 {0X20,0,0,0}
#define LED12 {0X10,0,0,0}
#define LED8 {0X08,0,0,0}
#define LED9 {0X04,0,0,0}
#define LED10 {0X02,0,0,0}
#define LED11 {0X01,0,0,0}

#define LED0 {0,0X80,0,0}
#define LED1 {0,0X40,0,0}
#define LED2 {0,0X20,0,0}
#define LED3 {0,0X10,0,0}
#define LED4 {0,0X08,0,0}
#define LED5 {0,0X04,0,0}
#define LED6 {0,0X02,0,0}
#define LED7 {0,0X01,0,0}

#define LED16 {0,0,0X80,0}
#define LED17 {0,0,0X40,0}
#define LED18 {0,0,0X20,0}
#define LED19 {0,0,0X10,0}
#define LED23 {0,0,0X08,0}
#define LED22 {0,0,0X04,0}
#define LED21 {0,0,0X02,0}
#define LED20 {0,0,0X01,0}

#define LED24 {0,0,0,0X80}
#define LED25 {0,0,0,0X40}
#define LED26 {0,0,0,0X20}
#define LED27 {0,0,0,0X10}
#define LED28 {0,0,0,0X08}
#define LED29 {0,0,0,0X04}
#define LED30 {0,0,0,0X02}
#define LED31 {0,0,0,0X01}

code LedMap[32][4]={LED0,LED1,LED2,LED3,LED4,LED5,LED6,LED7,LED8,LED9,LED10,LED11,LED12,LED13,LED14,LED15,LED16,
					LED17,LED18,LED19,LED20,LED21,LED22,LED23,LED24,LED25,LED26,LED27,LED28,LED29,LED30,LED31};
