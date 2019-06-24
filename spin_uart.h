#ifndef SPINUARTH
#define SPINUARTH
#include <reg52.h>	   //此文件中定义了51的一些特殊功能寄存器
#include"spin_common.h"
extern sem_t uart_recv;
extern sem_t uart_send;		//发送信号量
extern uchar recv_buff[10];//接收buff
extern uchar recv_buff_ptr;

uint spin_uart_init();//返回波特率
uint spin_write_uart(const char *buff,uint n);
uint spin_read_uart(char *buff);
uint spin_read_line(char *buff);
uchar spin_read_byte();
#endif