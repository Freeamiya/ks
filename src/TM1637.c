#include "TM1637.h"
void TM1637_Delay(void)
{
    unsigned char i;
    for (i = 0; i < 5; i++);  // 大约 5~10 微秒，根据主频略微调节
}

void TM1637_Start(void)
{
    SCLK_Pin = 1;
    SDO_Pin = 1;
    TM1637_Delay();
    SDO_Pin = 0;
    TM1637_Delay();
    SCLK_Pin = 0;
}

void TM1637_Stop(void)
{
    SCLK_Pin = 0;
    TM1637_Delay();
    SDO_Pin = 0;
    TM1637_Delay();
    SCLK_Pin = 1;
    TM1637_Delay();
    SDO_Pin = 1;
    TM1637_Delay();
}

void TM1637_WriteByte(unsigned char byte)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        SCLK_Pin = 0;
        TM1637_Delay();
        SDO_Pin = (byte & 0x01);
        TM1637_Delay();
        SCLK_Pin = 1;
        TM1637_Delay();
        byte >>= 1;
    }

    // 忽略 ACK 读取
    SCLK_Pin = 0;
    SDO_Pin = 1;
    TM1637_Delay();
    SCLK_Pin = 1;
    TM1637_Delay();
    SCLK_Pin = 0;
}

unsigned char seg_table[] = {
    0x3f, // 0
    0x06, // 1
    0x5b, // 2
    0x4f, // 3
    0x66, // 4
    0x6d, // 5
    0x7d, // 6
    0x07, // 7
    0x7f, // 8
    0x6f, // 9
    0x00  // blank
};

void TM1637_DisplayDigit(unsigned char BitAddr, unsigned char Data)
{
    TM1637_Start();
    TM1637_WriteByte(0x44); // 写入命令1：地址固定
    TM1637_Stop();

    TM1637_Start();
    TM1637_WriteByte(0xC0 | BitAddr); // 写入地址
    TM1637_WriteByte(seg_table[Data]);
    TM1637_Stop();

    TM1637_Start();
    TM1637_WriteByte(0x88 | 0x07); // 写入命令3：显示控制，全亮
    TM1637_Stop();
}

void TM1637_DisplayDecimal(int num)
{
    TM1637_DisplayDigit(0, num / 1000 % 10);
    TM1637_DisplayDigit(1, num / 100 % 10);
    TM1637_DisplayDigit(2, num / 10 % 10);
    TM1637_DisplayDigit(3, num % 10);
}

void TM1637_DisplayOff(void)
{
    TM1637_Start();
    TM1637_WriteByte(0x80); // 显示关
    TM1637_Stop();
}

void TM1637_Init(void)
{
    SCLK_Pin = 1;
    SDO_Pin = 1;
}
