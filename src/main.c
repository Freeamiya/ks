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

#define beep_pin P36 // 蜂鸣器引脚
#define LED_pin P20 // LED引脚

bit beep_enable = 0; // 蜂鸣器使能标志
unsigned int timer1_reload = 65536 - 1000; // 500us, 12MHz 时钟

// 位选引脚定义 (P2.4-P2.7对应G1-G4)
#define G1  P27
#define G2  P24
#define G3  P25
#define G4  P26
unsigned char display[4]; // 存储四位数字

// 共阳数码管段码表 (0-9)，低电平有效
unsigned char code SegCode[] = {
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
};

void delay_100ms(); //@12MHz
void InitTimer0(void);

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
        beep_pin = 0;         // 无声，默认拉高
    }
}

// 延时函数（大致延时n毫秒，适用于12MHz晶振）
void delay_ms(unsigned int n)
{
    unsigned int i, j;
    for (i = n; i > 0; i--)
        for (j = 110; j > 0; j--);
}

// 将距离值分解为四位数字
void get_digits(unsigned int dis, unsigned char *digits)
{
    digits[3] = dis / 1000;      // 千位
    digits[2] = (dis % 1000) / 100; // 百位
    digits[1] = (dis % 100) / 10;   // 十位
    digits[0] = dis % 10;           // 个位
    if(dis < 10)
    {
        digits[3] = 0; // 千位为0
        digits[2] = 0; // 百位为0
        digits[1] = 0; // 十位为0
    }
    else if(dis < 100)
    {
        digits[3] = 0; // 千位为0
        digits[2] = 0; // 百位为0
    }
    else if(dis < 1000)
    {
        digits[3] = 0; // 千位为0
    }
}

// 主函数
void main()
{
    InitTimer0(); // 初始化定时器
    while (1)
    {
        // 获取超声波测距值
        get_distance();
        // 分解距离值为四位数字
        get_digits(distance, display);
        // 动态扫描四位数码管
        for (int j = 0; j < 50; j++) // 快速刷新50次（总刷新时间约200ms）
        {
            for (int i = 0; i < 4; i++)
            {
                P0 = 0xFF;
                G1 = G2 = G3 = G4 = 1;
                P0 = SegCode[display[i]];
                switch (i)
                {
                    case 0: G1 = 0; break;
                    case 1: G2 = 0; break;
                    case 2: G3 = 0; break;
                    case 3: G4 = 0; break;
                }
                delay_ms(1); // 延时1ms，确保数码管显示稳定
            }
        }
        // delay_ms(10); // 延时10ms
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
    }
}