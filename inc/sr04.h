#ifndef __SR04_H__
#define __SR04_H__

#include <stdint.h>
#include <reg52.h>

extern unsigned long time;	// 用于存储时间
extern unsigned int distance;	// 距离值

// Define the SR04 pins
#define Trig P10		//触发信号控制端口
#define Echo P11		//回响信号接收端口

// Function 
void start_ranging(void);
void get_distance(void);

#endif // !__SR04_H__
