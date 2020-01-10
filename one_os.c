#include"one_os.h"
#include"spin_common.h"
#include"spin_userstart.h"
#include"spin_interupt.h"
#include"spin_timer.h"
#include"spin_exinterupt.h"
#include"spin_gpio.h"
#include"spin_uart.h"
#include"spin_watchdog.h"
#include"spin_pwrmgr.h"
#include"xprintf.h"
DATA unsigned int system_time0 = 0;	 //2个字节
DATA unsigned int system_time1 = 0;
//定时喂狗任务
void task1(){
	 spin_watchdog_clear();
}
//交替闪灯任务
void task2(){
	static uint recv_fre=0;
	recv_fre=~recv_fre;
	spin_set_gpio_bit_value(GPIO2,5,recv_fre);
}
//定时发送数据到串口
void task3(){
	xputs("os running\r\n"); 
}
//查询是否有串口数据
void task4(){

}

void task5(){
     //消息队列发送测试
  // fd = spin_cycle_queue_init();
   //spin_cycle_queue_push(fd,0x33);
}
void task6(){
     //消息队列接收测试
  // uchar content;
  // content = spin_cycle_queue_pop(fd);

}
void task7(){
   //内存管理测试
   //uchar addr;
   //memory_pool_init();
   //addr = spin_memory_malloc(2);
 
}
static Tasks task[]=   
{   
    {0,1,400,100,task1}, //喂狗		100ms
    {1,0,2000,500,task2}, //闪灯	500ms
	{2,0,4000,1000,task3},//发串口，1s
//	{3,0,2000,2000,task4},//读串口，500ms
//	{4,0,2000,2000,task5},//消息队列发送测试，500ms
//	{5,0,2000,2000,task6},//消息队列接收测试，500ms
//	{3,0,2000,2000,task7},//内存管理测试，500ms
};

void TaskHangup(unsigned char Task_Num)//任务挂起函数，参数就是你的任务编号
{ 
	task[Task_Num].Run=0;
}

void TaskRecovery(unsigned char Task_Num)//任务恢复函数，参数就是你的任务编号
{
	task[Task_Num].Run=1;
}

void TaskRemarks(void) //放在定时器中断里面
{
	unsigned char i;
	 system_time0++;
	for (i=0; i<TASK_MAX; i++)          
	{
		if (task[i].Timer)          
		{
			task[i].Timer--;        
			if (task[i].Timer == 0)  																														//所以用65500这个值保证溢出的时候也能执行下面的语句
			{
					task[i].Timer = task[i].ItvTime;
					task[i].Run = 1;           
			}
		}
	}
}

void TaskProcess(void)//放在你的while(1)循环里面
{
    unsigned char  i;
	for (i=0; i<TASK_MAX; i++)          
    {         
		if (task[i].Run)     
        {
             task[i].Run = 0;     
			 task[i].TaskHook();  
        }
    }   
	idle_mode();//进入低功耗模式		
}