#ifndef __PWM_H_
#define __PWM_H_

#include<reg52.h>
sbit pwm=P2^3;
extern volatile char pwm_counter; //250us自增1次，每秒自增4000次
extern const char top;//固定为100
PROCESS_NAME(pwm_led);
#endif