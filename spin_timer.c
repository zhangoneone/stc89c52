#include"spin_timer.h"
#include <reg52.h>	   //此文件中定义了51的一些特殊功能寄存器
#include<intrins.h>
//char local_timer_current_used = timer0;	 // 记录计时用的设备 ,-1代表未绑定设备,默认绑定
//char local_counter_current_used = timer1;//
//static uint* local_timer_current_callback;
//static uint* local_counter_current_callback;
//定时器0分成2个8位定时器使用，TL0提供时基，TH0提供定时。定时器1专门给串口作为波特率发生器，定时器2作为高级定时器
//系统时间，ms为单位
//0 低电平触发 1 下降沿触发
static void spin_counter_set(devices dev,TIMER_MODE_TYPES mode,uint time){
	  if(dev==timer0){
	  	TMOD &= 0xF0;
		TMOD |= 0x04;
		TMOD |= mode;
		if(mode==mode2){
			TL0=TH0=time;
			return;
	  	}
		TL0=(time&0xff); //高位截断还是低位截断?
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
		TL1=(time&0xff); //高位截断还是低位截断?
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
//传入溢出时间，传出要设置的初值
uint primary_value(uint overflow_time_us){
	  return (65536-(FSOC/12)*overflow_time_us);
}


//定时器0只能设为模式3
//定时器1只能设为模式2 已经被串口占用了
void spin_timer_set(devices dev,TIMER_MODE_TYPES mode,uint time){
	  if(dev==timer0){
		TMOD &= 0xF0;		//设置定时器模式
		TMOD |= mode;		//设置定时器模式
		switch(mode){
		 case mode0:
		 case mode1:TL0=(time&0xff);TH0=(time>>8);break;
		 case mode2:
		 case mode3:TH0=TL0=time;break;	//TH0作为定时器用，占据定时器1的中断。TL0作为系统时基用，在其他函数中单独设置
		 default:break;
		}
	  }
	  else if(dev==timer1){
	  	TMOD &= 0x0F;		//设置定时器模式
		TMOD |= (mode<<4);		//设置定时器模式
		switch(mode){
		 case mode0:
		 case mode1:TL1=(time&0xff);TH1=(time>>8);break;
		 case mode2:TL1=TH1=time;break;
		 case mode3:break;
		 default:break;
		}
	  }
}

SPIN_STATUS spin_counter_time(devices dev,uint time){ //作为计数器使用，脉冲来自外部引脚	,参数是次数
	  uint primary_values;
	  //设置初值
	  primary_values = primary_value(time);
	  spin_counter_set(dev,mode1,primary_values);
		//开始计次
	  spin_timer_start(dev);//通用
	  return spin_ok;
}
static void spin_systick_set(uint time){
	spin_timer_set(timer0,mode3,time);	
}
void uart_boaud(uint overtime){
   spin_timer_set(timer1,mode2,256-overtime);//自动装填模式
}
//为了产生系统时基
void spin_sysTick(uint callback){
	  //设置中断回调
	  inter_vector_function[timer0]=(uint)callback;
	  spin_systick_set(5);//每250us中断1次
		//开始计时
	  spin_timer_start(timer0);
	  spin_timer_start(timer1);
}

void delay2us(void)   //误差 0us
{
}
void delay3us(void)   //误差 0us
{
    _nop_();  //if Keil,require use intrins.h
}

void delay1ms(void)   //误差 0us
{
    unsigned char a,b;
    for(b=199;b>0;b--)
        for(a=1;a>0;a--);
}

void delay1s(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=167;c>0;c--)
        for(b=171;b>0;b--)
            for(a=16;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}