#include "contiki.h"
#include "ctimer.h"
#include "stimer.h"
#include "sys/clock.h"
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
#include"spin_1602.h"
#include"spin_at24c02.h"
#include"spin_pwm.h"
#include"spin_18b20.h"
#include"spin_ds1302.h"
void hardware_init(){
   //Ӳ����ʼ������
   spin_set_work_register_group(0);
   spin_interupt_init();
   spin_interupt_enable();
   spin_uart_init();
   spin_watchdog_enable();
   spin_watchdog_overtime(128,FSOC); //�������ʱ��ms ���ڴ�Լ��4000
   spin_exint0_start(down_eage);
   spin_exint1_start(low_vol);
   //���ڲ���
   xdev_in(u_getc);
   xdev_out(u_putc);
   ds1302_rtc_set_time(&default_time);
}
PROCESS(led,"led");//����led��ת����
PROCESS_THREAD(led, ev, dataa)
{
	static struct etimer et;
    PROCESS_BEGIN();
	//��ʱ1s��ʱ��
	etimer_set(&et,CLOCK_SECOND/2);
	while(1)
	{
		//�ȴ�1s
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));	
	    //����led
		spin_set_gpio_bit_value(GPIO2,2,0);
		//�ȴ�1s
		etimer_restart(&et);
    	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		//�ر�led
		spin_set_gpio_bit_value(GPIO2,2,1);
		etimer_restart(&et);
	}
   PROCESS_END();
}
PROCESS(dog,"dog");//���Ź�����
PROCESS_THREAD(dog, ev, dataa) //����ĺ���ʵ��һ���������壬�������˲���struct pt *process_pt���������ṹ���ָ��
{
	static struct etimer et;
    PROCESS_BEGIN();   //�ú������˱�־λ������ȡ��������Ϣ
	//��ʱ1s��ʱ��
	etimer_set(&et,CLOCK_SECOND*2);
	while(1)
	{
		//�ȴ�1s
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));//���ﱣ�������������������˱�־λ��Ȼ�������ء���һ����������ʱ��ִ����һ��	
	    spin_watchdog_clear();//ι��
		etimer_restart(&et);
	}
   PROCESS_END();  //�ú������˱�־λ����������������Ϣ��Ȼ��������
}
PROCESS(holdlive,"holdlive");//��Ķ�ʱʱ����Ϊ���ƿ�51�ϵ�contiki bug
PROCESS_THREAD(holdlive, ev, dataa) //����ĺ���ʵ��һ���������壬�������˲���struct pt *process_pt���������ṹ���ָ��
{
	static struct etimer et;
    PROCESS_BEGIN();   //�ú������˱�־λ������ȡ��������Ϣ
	//��ʱ���
	etimer_set(&et,CLOCK_SECOND*33554430);
	while(1)
	{
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));//���ﱣ�������������������˱�־λ��Ȼ�������ء���һ����������ʱ��ִ����һ��	
		etimer_restart(&et);
	}
   PROCESS_END();  //�ú������˱�־λ����������������Ϣ��Ȼ��������
}
process_event_t lcd_update1;
process_event_t lcd_update2;
int
main(void)
{
  hardware_init();
  clock_init();
  rtimer_init();
  ctimer_init();
  process_init();
  lcd_update1 = process_alloc_event();	//��ʼ���¼�
  lcd_update2 = process_alloc_event();	//��ʼ���¼�
 /* start services */
  process_start(&etimer_process, NULL);
  process_start(&holdlive, NULL); //���һ��Ҫ��etimer��ʼ������
  process_start(&dog, NULL);
  process_start(&led, NULL);
  process_start(&lcd, NULL);
  process_start(&pwm_led,NULL);
  process_start(&at24c02,NULL); 
  process_start(&_18b20,NULL);
  process_start(&ds1302,NULL);
  for(;;){
  	if(process_run()==0)idle_mode();
  }
  return 0;
}
