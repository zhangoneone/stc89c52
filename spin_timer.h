#ifndef SPINTIMERH
#define SPINTIMERH

#include"spin_common.h"
#include"spin_interupt.h"
typedef enum{
   mode0=0,
   mode1,
   mode2,
   mode3,
}TIMER_MODE_TYPES;

void spin_timer_start(devices dev);
void spin_timer_stop(devices dev);
SPIN_STATUS spin_counter_time(devices dev,uint time); //作为计数器使用，脉冲来自外部引脚	,参数是次数
void uart_boaud(uint overtime);//给uart作为波特率发生器
void delay2us(void);
void delay3us(void);
void delay1ms(void);
void delay1s(void);
void spin_sysTick();
void delayus(unsigned char us);
void delayms(unsigned char ms);
#endif