#ifndef SPINGPIOH
#define SPINGPIOH
#include <reg52.h>	   //此文件中定义了51的一些特殊功能寄存器
#include"spin_common.h"
void  spin_set_gpio_value(uchar gpio,uchar value);
uchar spin_get_gpio_value(uchar gpio);

void  spin_set_gpio_bit_value(uchar gpio,uchar index,uchar value);
uchar spin_get_gpio_bit_value(uchar gpio,uchar index);

#endif