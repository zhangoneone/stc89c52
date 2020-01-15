#ifndef SPININTERUPTH
#define SPININTERUPTH
#include <reg52.h>	   //此文件中定义了51的一些特殊功能寄存器
#include"spin_common.h"
extern uint* inter_vector_function[inter_num]; //放到这里，方便调用该头文件的文件,需要调用提供中断回调函数
typedef void*(interupt_callback)(void *);
void spin_interupt_init();
void spin_interupt_enable();
void spin_interupt_disable();
void spin_interupt_open(INTER_LIST num,INTER_PRORITY prority);
void spin_interupt_close(INTER_LIST num);
void spin_interupt_set_params(INTER_LIST num,uchar params);	 //不好用
#endif