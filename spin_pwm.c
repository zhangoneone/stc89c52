#include "spin_pwm.h"
#include "clock.h"
#include"etimer.h"
#include"process.h"
volatile char revert_value;
//ռ�ձȵ�λ10% 0~100%
#define set_rate(rate)\
	revert_value=rate

void pwm_start(){
  if(pwm_counter<revert_value)pwm=0;
  else pwm=1;
}
PROCESS(pwm_led,"pwm_led");//ͨ��pwmʵ�ֺ�����
extern process_data_t global_dataa;
PROCESS_THREAD(pwm_led, ev, dataa)
{
	static char rate=0;
	static char dir=0;//0�� 1��
	static struct etimer et;
    PROCESS_BEGIN();
	//��ʱ1/16 s��ʱ��
	etimer_set(&et,CLOCK_SECOND/6);
	while(1)
	{
		//�ȴ�1/16s
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		if(dir)
			revert_value--;
		else
			revert_value++;
		if(revert_value==top/3)dir=1;//�����Ϊ��
		else if(revert_value==0)dir=0;//�����Ϊ��
		else dir=dir;//���򲻱�
		etimer_restart(&et);
	}
   PROCESS_END();
}