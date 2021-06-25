#include"spin_gpio.h"

#define local_set(gpio,value)	P##gpio=value
#define local_get(gpio)			P##gpio
//#define local_set_bit(gpio,index,value)	P##gpio##_##index=!!value

void  spin_set_gpio_value(GPIOS gpio,uchar value){
    switch(gpio){
   				case 0:local_set(0,value);break;
				case 1:local_set(1,value);break;
				case 2:local_set(2,value);break;
				case 3:local_set(3,value);break;
				default:break;
   	}
}
uchar spin_get_gpio_value(GPIOS gpio){
	 uchar temp = 0;
	 switch(gpio){
   				case 0:temp = local_get(0);break;
				case 1:temp = local_get(1);break;
				case 2:temp = local_get(2);break;
				case 3:temp = local_get(3);break;
				default:break;
   	}
	return temp;
}
  //index start from 0
void  spin_set_gpio_bit_value(GPIOS gpio,uchar index,uchar value){
	uchar temp = 0;
	if(!!value)
		temp=spin_get_gpio_value(gpio) | (0x01<<index);
	else
		temp=spin_get_gpio_value(gpio)& (~(0x01<<index));
   switch(gpio){
   				case 0:local_set(0,temp);break;
				case 1:local_set(1,temp);break;
				case 2:local_set(2,temp);break;
				case 3:local_set(3,temp);break;
				default:break;
   	}
}

uchar spin_get_gpio_bit_value(GPIOS gpio,uchar index){
     uchar temp = 0;
	 switch(gpio){
   				case 0:temp = local_get(0);break;
				case 1:temp = local_get(1);break;
				case 2:temp = local_get(2);break;
				case 3:temp = local_get(3);break;
				default:break;
   	}
	return !!(temp & (0x01<<index));
}