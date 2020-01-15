#ifndef SPININTERUPTH
#define SPININTERUPTH
#include"spin_common.h"
#define spin_interupt_enable() (EA=1)
#define spin_interupt_disable() (EA=0)
extern uint DATA inter_vector_function[inter_num]; //放到这里，方便调用该头文件的文件,需要调用提供中断回调函数
extern mutex_t it0,it1,it2,it3,it4,it5,it6,it7;
typedef void(*interupt_callback)(void);
void spin_interupt_init();
void spin_interupt_open(uchar inter_index,uchar prority);
void spin_interupt_close(uchar inter_index);
#endif