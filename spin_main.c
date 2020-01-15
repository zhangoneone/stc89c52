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
#include"one_os.h"
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

void hardware_init(){
    //注册用户中断函数
   inter_vector_function[out_int0]=(uchar *)eint0_user;
   inter_vector_function[timer0]=(uchar *)timer0_user;
   inter_vector_function[out_int1]=(uchar *)eint1_user;
   inter_vector_function[timer1]=(uchar *)timer1_user;
   inter_vector_function[serial]=(uchar *)uart_user;
   //硬件初始化设置
   spin_set_work_register_group(0);
   spin_interupt_init();
   spin_interupt_enable();
   spin_uart_init();
   spin_watchdog_enable();
   spin_watchdog_overtime(32,FSOC); //返回溢出时间ms 现在大约是1000
   spin_exint0_start(down_eage);
   spin_exint1_start(low_vol);
   //串口测试
   spin_write_uart("51 start",strlen("51 start"));//接收到的数据回写
   //定时器相关测试
   //spin_counter_time(50);//计数50次，达到就中断。和串口共用timer1
   //spin_timer_sec(1);  //定时1s
   spin_sysTick(TaskRemarks);//产生时基，开始系统,时基250us
}
void main(void){
   hardware_init();
   while(1){
   		TaskProcess();
   }
}

