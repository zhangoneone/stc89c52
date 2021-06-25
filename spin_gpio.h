#ifndef SPINGPIOH
#define SPINGPIOH
#include <reg52.h>	   //���ļ��ж�����51��һЩ���⹦�ܼĴ���
#include"spin_common.h"

#define local_set(gpio,value)	(P##gpio=value)
#define local_get(gpio)			(P##gpio)
#define local_set_bit(gpio,index,value)	(P##gpio##^##index=!!value)
#define local_get_bit(gpio,index)	   	(P##gpio##^##index)

void  spin_set_gpio_value(GPIOS gpio,uchar value);
uchar spin_get_gpio_value(GPIOS gpio);

void  spin_set_gpio_bit_value(GPIOS gpio,uchar index,uchar value);
uchar spin_get_gpio_bit_value(GPIOS gpio,uchar index);

#endif