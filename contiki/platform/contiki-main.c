#include "contiki.h"
#include "stack.h"
#include "sys/clock.h"
#include "sys/autostart.h"
#include "spin_gpio.h"
PROCESS(led, "led");//定义led翻转任务
AUTOSTART_PROCESSES(&led);//注册task
PROCESS_THREAD(led, ev, dataa)
{
	static struct etimer xdata et;
    PROCESS_BEGIN();
	while(1)
	{	
	    //点亮led
		spin_set_gpio_bit_value(GPIO2,1,0);
		//延时1s
		etimer_set(&et,CLOCK_SECOND*1);
    	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		//关闭led
		spin_set_gpio_bit_value(GPIO2,1,1);
		//延时1s
		etimer_set(&et,CLOCK_SECOND*1);
    	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	}
   PROCESS_END();
}
int
main(void)
{
  clock_init();
  while(1);
  process_init();
 /* start services */
  process_start(&etimer_process, NULL);
  autostart_start(autostart_processes);
  for(;;)process_run();
  return 0;
}
