#include"spin_timer.h"
#include <reg52.h>	   //���ļ��ж�����51��һЩ���⹦�ܼĴ���
#include<intrins.h>
//char local_timer_current_used = timer0;	 // ��¼��ʱ�õ��豸 ,-1����δ���豸,Ĭ�ϰ�
//char local_counter_current_used = timer1;//
//static uint* local_timer_current_callback;
//static uint* local_counter_current_callback;
//��ʱ��0�ֳ�2��8λ��ʱ��ʹ�ã�TL0�ṩʱ����TH0�ṩ��ʱ����ʱ��1ר�Ÿ�������Ϊ�����ʷ���������ʱ��2��Ϊ�߼���ʱ��
//ϵͳʱ�䣬msΪ��λ
//0 �͵�ƽ���� 1 �½��ش���
static void spin_counter_set(devices dev,TIMER_MODE_TYPES mode,uint time){
	  if(dev==timer0){
	  	TMOD &= 0xF0;
		TMOD |= 0x04;
		TMOD |= mode;
		if(mode==mode2){
			TL0=TH0=time;
			return;
	  	}
		TL0=(time&0xff); //��λ�ضϻ��ǵ�λ�ض�?
	  	TH0=time>>8;
	  }
	  else if(dev==timer1){
	  	TMOD &= 0x0F;
		TMOD |= 0x40;
		TMOD |= (mode<<4);
		if(mode==mode2){
			TL1=TH1=time;
			return;
	  	}
		TL1=(time&0xff); //��λ�ضϻ��ǵ�λ�ض�?
	  	TH1=time>>8;
	  }
}


void spin_timer_start(devices dev){
	  spin_interupt_enable();
	  spin_interupt_open(dev,high);
	  if(dev==timer0)TR0=1;
	  if(dev==timer1)TR1=1;
}

void spin_timer_stop(devices dev){
	  if(dev==timer0)TR0=0;
	  if(dev==timer1)TR1=0;
	  spin_interupt_close(dev);
}
//�������ʱ�䣬����Ҫ���õĳ�ֵ
uint primary_value(uint overflow_time_us){
	  return (65536-(FSOC/12)*overflow_time_us);
}


//��ʱ��0ֻ����Ϊģʽ3
//��ʱ��1ֻ����Ϊģʽ2 �Ѿ�������ռ����
void spin_timer_set(devices dev,TIMER_MODE_TYPES mode,uint time){
	  if(dev==timer0){
		TMOD &= 0xF0;		//���ö�ʱ��ģʽ
		TMOD |= mode;		//���ö�ʱ��ģʽ
		switch(mode){
		 case mode0:
		 case mode1:TL0=(time&0xff);TH0=(time>>8);break;
		 case mode2:
		 case mode3:TH0=TL0=time;break;	//TH0��Ϊ��ʱ���ã�ռ�ݶ�ʱ��1���жϡ�TL0��Ϊϵͳʱ���ã������������е�������
		 default:break;
		}
	  }
	  else if(dev==timer1){
	  	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
		TMOD |= (mode<<4);		//���ö�ʱ��ģʽ
		switch(mode){
		 case mode0:
		 case mode1:TL1=(time&0xff);TH1=(time>>8);break;
		 case mode2:TL1=TH1=time;break;
		 case mode3:break;
		 default:break;
		}
	  }
}

SPIN_STATUS spin_counter_time(devices dev,uint time){ //��Ϊ������ʹ�ã����������ⲿ����	,�����Ǵ���
	  uint primary_values;
	  //���ó�ֵ
	  primary_values = primary_value(time);
	  spin_counter_set(dev,mode1,primary_values);
		//��ʼ�ƴ�
	  spin_timer_start(dev);//ͨ��
	  return spin_ok;
}
static void spin_systick_set(uint time){
	spin_timer_set(timer0,mode3,time);	
}
void uart_boaud(uint overtime){
   spin_timer_set(timer1,mode2,256-overtime);//�Զ�װ��ģʽ
}
//Ϊ�˲���ϵͳʱ��
void spin_sysTick(){
	  spin_systick_set(5);//ÿ250us�ж�1��
		//��ʼ��ʱ
	  spin_timer_start(timer0);
	  spin_timer_start(timer1);
}

void delay2us(void)   //��� 0us
{
}
void delay3us(void)   //��� 0us
{
    _nop_();  //if Keil,require use intrins.h
}

void delay1ms(void)   //��� 0us
{
    unsigned char a,b;
    for(b=199;b>0;b--)
        for(a=1;a>0;a--);
}

void delay1s(void)   //��� 0us
{
    unsigned char a,b,c;
    for(c=167;c>0;c--)
        for(b=171;b>0;b--)
            for(a=16;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}

void delayus(unsigned char us){
	unsigned char i = 0;
	if(us==1)return;
	if(us==2)return;
	for(;i<us-2;i++)
		_nop_();
	return;
}

void delayms(unsigned char ms){
	unsigned char i =0;
	 for(;i<ms;i++)
	 	delay1ms();
	return;
}