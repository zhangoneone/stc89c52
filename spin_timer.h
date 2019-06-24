#ifndef SPINTIMERH
#define SPINTIMERH
#include <reg52.h>	   //此文件中定义了51的一些特殊功能寄存器
#include"spin_common.h"
#include"spin_interupt.h"
//#include<stdarg.h>
typedef enum{
   mode0=0,
   mode1,
   mode2,
   mode3,
}TIMER_MODE_TYPES;

SPIN_STATUS spin_timer_us(uint us);	//作为定时器使用时，脉冲频率是振荡器的12分频。振荡器频率是12MHZ,指令周期是1us
SPIN_STATUS spin_timer_ms(uint ms);
SPIN_STATUS spin_timer_sec(uint sec);
SPIN_STATUS spin_counter_time(uint time); //作为计数器使用，脉冲来自外部引脚	,参数是次数
//传入溢出时间，传出要设置的初值
uint primary_value(uint overflow_time_us);
void spin_timer_set(INTER_LIST dev,TIMER_MODE_TYPES mode,uint time);
void spin_timer_start(INTER_LIST dev);
void spin_timer_stop(INTER_LIST dev);
void spin_timer_for_uart(INTER_LIST dev,uint overtime);//给uart作为波特率发生器
void delay2us(void);
void delay3us(void);
void delay1ms(void);
void delay1s(void);
#endif