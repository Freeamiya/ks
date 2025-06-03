#ifndef __TM1637_H
#define __TM1637_H

#include <reg52.h>

// 引脚定义
#define SCLK_Pin P11
#define SDO_Pin  P12

// 基本功能函数声明
void TM1637_Delay(void);
void TM1637_Init(void);
void TM1637_DisplayDigit(unsigned char BitAddr, unsigned char Data);
void TM1637_DisplayDecimal(int num);
void TM1637_DisplayOff(void);

#endif

