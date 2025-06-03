#include "sr04.h"
unsigned long time = 0;
unsigned int distance = 0;	//距离值
/* 启动超声波程序 */
void start_ranging(void)
{
	Trig = 1;			//启动一次模块
    unsigned char i = 23;
    while (--i);
	Trig = 0;
}

/* 获取超声波测距 */
void get_distance(void)
{
    start_ranging();	//启动超声波模块
	while(!Echo);		//等待超声波模块输出IO拉高 
	TR0 = 1;			//开启定时器计时
	while(Echo);		//等待超声波模块输出IO拉低
	TR0 = 0; 			//关闭定时器计时

    /* 计算距离 */
	time = TH0 * 256 + TL0;	//得到的时间，专业写法应该是这样：time = TH0 << 8 | TL0
	TH0 = 0;				//定时计数器清零	
	TL0 = 0;
	
	distance = (time * 1.7) / 100; //算出来是CM		
}