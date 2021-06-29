#ifndef SPINUARTH
#define SPINUARTH
#include <reg52.h>	   //此文件中定义了51的一些特殊功能寄存器
#include"spin_common.h"

uint spin_uart_init();//返回波特率
uint spin_write_uart(const char idata *buff,uint n);
uint spin_read_uart(char idata *buff);
#endif