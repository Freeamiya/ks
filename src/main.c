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

void delay_100ms(); //@12MHz

void main(void)
{
    TM1637_Init();
    while (1)
    {
        TM1637_DisplayDecimal(1234); // 显示数字
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