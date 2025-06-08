#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned int

#define sounder P13 // 蜂鸣器控制脚
#define KEY_PAUSE  P32 // 暂停按键
#define KEY_NEXT   P33 // 切歌按键

unsigned char num, beat;
unsigned int timer, index = 0;
unsigned char song_index = 0; // 当前歌曲索引
bit paused = 0;               // 暂停标志

// 原频率表（单位: Hz）
unsigned int code freq[] = {
    466,523,578,659,698,784,880,    // 低音
    932,1047,1175,1319,1397,1568,1760, // 中音
    1865,2094,2351,2633,2792,3134,3517 // 高音
};

// 音符编号（两首歌曲）
unsigned char code yinfu1[] = {
    5,1,2,2,3,3,2,3,3,2,3,5,3,1,1,5,6,3,2,1,2,
    3,5,1,2,2,3,3,2,3,3,2,3,5,3,1,1,7,6,3,2,1,2,
    1,1,7,6,1,2,1,2,3,5,2,3,1,1,7,6,1,2,3,2,1,2,
    1,7,5,1,3,2,1,100
};

unsigned char code yinchang1[] = {
    2,2,2,2,2,4,2,2,4,2,2,2,2,4,2,2,3,2,4,2,2,
    4,2,2,2,2,2,2,4,2,2,4,2,2,2,2,4,2,2,3,2,4,2,2,
    4,2,2,4,4,4,2,2,2,2,2,2,4,2,2,4,4,2,1,1,2,2,
    2,2,2,2,4,4,4,100
};

unsigned char code yingao1[] = {
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,
    2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,
    2,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,
    3,2,2,2,2,2,2,100
};

// 第二首（演示用简单）
unsigned char code yinfu2[] = {
    1,2,3,4,5,6,7,100
};
unsigned char code yinchang2[] = {
    2,2,2,2,2,2,4,100
};
unsigned char code yingao2[] = {
    1,1,1,1,1,1,1,100
};

// 延时函数
void delay_ms(unsigned int ms) 
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 114; j++);
}

// 定时器0初始化
void Time0_Init()
{
    TMOD = 0x01;
    IE   = 0x8F; // 启动定时器0中断 + EX0 和 EX1 中断
    IT0  = 1;    // 外部中断0下降沿触发（暂停）
    IT1  = 1;    // 外部中断1下降沿触发（切歌）
}

// 定时器0中断服务函数
void Time0_Int() interrupt 1
{
    TH0 = timer / 256;
    TL0 = timer % 256;
    sounder = !sounder;
}

// 外部中断0：控制暂停/继续播放
void External0_ISR() interrupt 0
{
    delay_ms(20); // 简易防抖
    if (KEY_PAUSE == 0) {
        paused = !paused;
    }
    while (KEY_PAUSE == 0);
}

// 外部中断1：切换下一首歌
void External1_ISR() interrupt 2
{
    delay_ms(20);
    if (KEY_NEXT == 0) {
        song_index++;
        if (song_index >= 2) song_index = 0;
        index = 0;
        paused = 0; // 切歌自动播放
    }
    while (KEY_NEXT == 0);
}

// 播放自定义歌曲
void Play_CustomSong()
{
    unsigned char current_note;
    while (1)
    {
        if (paused) {
            TR0 = 0; // 暂停关闭蜂鸣器
            continue;
        }

        if (song_index == 0) current_note = yinfu1[index];
        else current_note = yinfu2[index];

        if (current_note == 100) {
            TR0 = 0;
            break;
        }

        if (current_note == 0) {
            TR0 = 0;
        } else {
            if (song_index == 0)
                num = yinfu1[index] - 1 + (yingao1[index] - 1) * 7;
            else
                num = yinfu2[index] - 1 + (yingao2[index] - 1) * 7;

            timer = 65536 - (1000000 / (freq[num] * 2));
            TH0 = timer / 256;
            TL0 = timer % 256;
            TR0 = 1;
        }

        if (song_index == 0) beat = yinchang1[index];
        else beat = yinchang2[index];

        delay_ms(beat * 100);
        index++;
    }
    index = 0;
    TR0 = 0;
}

// 主函数
void main()
{
    sounder = 1;
    Time0_Init();
    while(1)
    {
        Play_CustomSong();
    }
}
