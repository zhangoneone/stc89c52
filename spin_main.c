#include<reg52.h>
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
#include "xprintf.h"

void hardware_init(){
//	spin_set_sp(0x07);//工作组0之后的ram都归堆栈使用
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
   xdev_in(u_getc);
   xdev_out(u_putc);
   spin_sysTick(TaskRemarks);//产生时基，开始系统,时基250us
}
void main(void){
   hardware_init();
   while(1){
   		TaskProcess();
   }
}

