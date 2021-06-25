#include "spin_pwm.h"
#include "clock.h"
#include"etimer.h"
#include"process.h"
volatile char revert_value;
//占空比单位10% 0~100%
#define set_rate(rate)\
	revert_value=rate

void pwm_start(){
  if(pwm_counter<revert_value)pwm=0;
  else pwm=1;
}
PROCESS(pwm_led,"pwm_led");//通过pwm实现呼吸灯
extern process_data_t global_dataa;
PROCESS_THREAD(pwm_led, ev, dataa)
{
	static char rate=0;
	static char dir=0;//0增 1减
	static struct etimer et;
    PROCESS_BEGIN();
	//延时1/16 s的时钟
	etimer_set(&et,CLOCK_SECOND/6);
	while(1)
	{
		//等待1/16s
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		if(dir)
			revert_value--;
		else
			revert_value++;
		if(revert_value==top/3)dir=1;//方向改为减
		else if(revert_value==0)dir=0;//方向改为增
		else dir=dir;//方向不变
		etimer_restart(&et);
	}
   PROCESS_END();
}