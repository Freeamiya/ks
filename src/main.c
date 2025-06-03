/*************************************
 * 
 *        89c52 sdcc blink demo
 * 
 * CPU: 89C52
 * FREQ: 12MHz
 * PIN: P00
 * 
 * ***********************************
*/

#include <reg52.h>
#include <stdio.h>
#include "TM1637.h"
#include "sr04.h"

#define beep_pin P13 // 蜂鸣器引脚
#define LED_pin P00 // LED引脚

bit beep_enable = 0; // 蜂鸣器使能标志
unsigned int timer1_reload = 65536 - 2000; // 500us, 12MHz 时钟

void delay_100ms(); //@12MHz
void delay_1ms();   //@12MHz
void InitTimer0(void);

void main(void)
{
    TM1637_Init();
    InitTimer0(); // 初始化定时器

    while (1)
    {
        get_distance(); // 获取距离
        TM1637_DisplayDecimal(distance); // 显示距离

        if (distance < 10) // 如果距离小于10cm
        {
            LED_pin = 0;
            beep_enable = 1; // 启用蜂鸣器
        }
        else
        {
            LED_pin = 1;
            beep_enable = 0; // 关闭蜂鸣器
        }

        delay_100ms(); // 主循环延时，不影响蜂鸣器方波
    }
}

void delay_100ms()
{
    unsigned char i, j;

    i = 195;
    j = 138;

    do
    {
        while (--j)
            ;
    } while (--i);
}

// 延时1毫秒
void delay_1ms()
{
    unsigned char i, j;

    i = 2; // 12MHz时钟下，1ms大约需要2个循环
    j = 250; // 每个循环大约需要4个机器周期

    do
    {
        while (--j)
            ;
    } while (--i);
}

void InitTimer0(void)
{
    TMOD = 0x11;	//配置定时器0为工作方式1，定时器1位工作方式1
	TH0 = 0;
    TL0 = 0;
    TH1 = timer1_reload >> 8;
    TL1 = timer1_reload;

    EA = 1;			//打开总中断
    TR1 = 1;        // 启动 T1
	ET1 = 1;		//打开定时器0中断
    TR0 = 0;		
}

void timer1_ISR(void) interrupt 3
{
    TH1 = timer1_reload >> 8;
    TL1 = timer1_reload;

    if (beep_enable)
    {
        beep_pin = !beep_pin; // 翻转蜂鸣器引脚
    }
    else
    {
        beep_pin = 1;         // 无声，默认拉高
    }
}
