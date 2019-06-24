//#include <reg51.h>	   //此文件中定义了51的一些特殊功能寄存器
#include<reg52.h>
#include<string.h>
#include<stdio.h>
#include"spin_common.h"
#include"spin_userstart.h"
#include"spin_interupt.h"
#include"spin_timer.h"
#include"spin_exinterupt.h"
#include"spin_gpio.h"
#include"spin_uart.h"
#include"spin_watchdog.h"
#include"spin_pwrmgr.h"

void eint0_user(void){
	spin_set_gpio_bit_value(GPIO2,0,0);
}
void timer0_user(void){
	static uint islive=0;
	islive=~islive;
	spin_set_gpio_bit_value(GPIO2,1,islive);
}
void eint1_user(void){
	spin_set_gpio_bit_value(GPIO2,2,0);
}
void timer1_user(void){
	spin_set_gpio_bit_value(GPIO2,3,0);
}
void uart_user(void){ //接收到1个字符就进入这里
	static uint recv_fre=0;
	recv_fre=~recv_fre;
	spin_set_gpio_bit_value(GPIO2,4,recv_fre);
}
void main(void){
   uchar recv_buff[10];
   uchar ret;
   spin_set_work_register_group(0);
   spin_interupt_init();
   spin_interupt_enable();
   spin_uart_init();//1200
   spin_watchdog_enable();
   ret = spin_watchdog_overtime(4,FSOC);
   recv_buff[0] = ret;
   spin_write_uart(recv_buff,strlen(recv_buff));//接收到的数据回写
   inter_vector_function[out_int0]=(uchar *)eint0_user;
   inter_vector_function[timer0]=(uchar *)timer0_user;
   inter_vector_function[out_int1]=(uchar *)eint1_user;
   inter_vector_function[timer1]=(uchar *)timer1_user;
   inter_vector_function[serial]=(uchar *)uart_user;
   //spin_counter_time(50);//计数50次，达到就中断。和串口共用timer1
   spin_exint0_start(down_eage);
   spin_exint1_start(low_vol);
   spin_timer_sec(1);  //定时1s
   while(1){
   }
}

