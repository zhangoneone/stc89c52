#include"spin_timer.h"
#include<intrins.h>
char local_timer_current_used = timer0;	 // 记录计时用的设备 ,-1代表未绑定设备,默认绑定
char local_counter_current_used = timer1;//
static uint* local_timer_current_callback;
static uint* local_counter_current_callback;

  //私有函数

static void spin_timer_close(){
	spin_timer_stop(local_timer_current_used);
	spin_interupt_close(local_timer_current_used);
	local_timer_current_used = timer0;
}
static void spin_counter_close(){
	spin_timer_stop(local_counter_current_used);//可以通用
	spin_interupt_close(local_counter_current_used);
	local_counter_current_used = timer1;
}

//0 低电平触发 1 下降沿触发
static void spin_counter_set(TIMER_MODE_TYPES mode,uint time){
	  if(local_counter_current_used==timer0){
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
	  else if(local_counter_current_used==timer1){
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
static uint local_remain_ms = 0;
//这里是为了在计时结束前，进入这里，而不进入用户设置的中断回调函数
static uint spin_timer_local_callback(){
	uint ret = 0;
	uint primary_values;
	if(local_remain_ms==0){//计时结束
		spin_timer_close();
		inter_vector_function[local_timer_current_used]=local_timer_current_callback;
		((interupt_callback)inter_vector_function[local_timer_current_used])();
	}
	else{ //计时未结束
		if(local_remain_ms>60){//1次无法完成
		   //设置初值
	  	   primary_values = primary_value(60*1000);
		   local_remain_ms-=60;
		}
		else{  //1次即可完成
			//设置初值
	  	    primary_values = primary_value(primary_values*1000);
		    local_remain_ms-=local_remain_ms;
		}

	  	spin_timer_set(local_timer_current_used,mode1,primary_values);
		//开始计时
		spin_timer_start(local_timer_current_used);
		return spin_ok; 
	}
}

void spin_timer_start(INTER_LIST dev){
	  if(dev==timer0)TR0=1;
	  if(dev==timer1)TR1=1;
}

void spin_timer_stop(INTER_LIST dev){
	  if(dev==timer0)TR0=0;
	  if(dev==timer1)TR1=0;
}
//传入溢出时间，传出要设置的初值
uint primary_value(uint overflow_time_us){
	  return (65536-(FSOC/12)*overflow_time_us);
}


void spin_timer_set(INTER_LIST dev,TIMER_MODE_TYPES mode,uint time){
	  if(dev==timer0){
		TMOD &= 0xF0;		//设置定时器模式
		TMOD |= mode;		//设置定时器模式
		if(mode==mode2){
			TL0=TH0=time;
			return;
	  	}
		TL0=(time&0xff); //高位截断还是低位截断?
	  	TH0=time>>8;
	  }
	  else if(dev==timer1){
	  	TMOD &= 0xF0;		//设置定时器模式
		TMOD |= (mode<<4);		//设置定时器模式
		if(mode==mode2){
			TL1=TH1=time;
			return;
	  	}
		TL1=(time&0xff); //高位截断还是低位截断?
	  	TH1=time>>8;
	  }
}

//us 0~65535
SPIN_STATUS spin_timer_us(uint us){	//作为定时器使用时，脉冲频率是振荡器的12分频。振荡器频率是12MHZ,指令周期是1us
	  uint primary_values;
	  //设置中断回调
	  local_timer_current_callback = inter_vector_function[local_timer_current_used];
	  inter_vector_function[local_timer_current_used]=(uchar *)spin_timer_local_callback;

	  spin_interupt_init();
	  spin_interupt_enable();
	  //设置初值
	  primary_values = primary_value(us);
	  spin_timer_set(local_timer_current_used,mode1,primary_values);
	  spin_interupt_open(local_timer_current_used,high);
		//开始计时
	  spin_timer_start();
	  return spin_ok;
}
//ms 0~65535
SPIN_STATUS spin_timer_ms(uint ms){
	SPIN_STATUS ret = 0;
	local_remain_ms = ms;
	if(local_remain_ms>60){//1次无法完成
		   ret = spin_timer_us(60*1000);
		   local_remain_ms-=60;
		}
	else{  //1次即可完成
			ret = spin_timer_us(local_remain_ms*1000);
			local_remain_ms -= local_remain_ms;
		}
	return ret;
}
//sec 0~65
SPIN_STATUS spin_timer_sec(uint sec){
	SPIN_STATUS ret = 0;
	ret = spin_timer_ms(sec*1000);
	return ret;
}
//这里是为了在计数结束时，进入这里做善后工作，而推迟进入用户设置的中断回调函数
static uint spin_counter_local_callback(){
	spin_counter_close();
	inter_vector_function[local_counter_current_used]=local_counter_current_callback;
	((interupt_callback)inter_vector_function[local_counter_current_used])();
	return spin_ok;
}

SPIN_STATUS spin_counter_time(uint time){ //作为计数器使用，脉冲来自外部引脚	,参数是次数
	  uint primary_values;
	  spin_interupt_init();
	  spin_interupt_enable();
	  //设置初值
	  primary_values = primary_value(time);
	  //设置中断回调
	  local_counter_current_callback = inter_vector_function[local_counter_current_used];
	  inter_vector_function[local_counter_current_used]=(uchar *)spin_counter_local_callback;
	  spin_counter_set(mode1,primary_values);
	  spin_interupt_open(local_counter_current_used,high);
		//开始计次
	  spin_timer_start(local_counter_current_used);//通用
	  return spin_ok;
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
void spin_timer_for_uart(INTER_LIST dev,uint overtime){
	uint time;//初值
   //打开定时器
   spin_interupt_init();
   spin_interupt_enable();
   time = primary_value(overtime);

   spin_timer_set(dev,mode2,time);//自动装填模式
   //禁止timer1中断
   spin_interupt_close(dev);
   spin_timer_start(dev);

}
//为了产生系统时基
void spin_sysTick(void(*callback)(void)){
	  uint primary_values;
	  //设置中断回调
	  local_timer_current_callback = inter_vector_function[local_timer_current_used];
	  inter_vector_function[local_timer_current_used]=(uchar *)callback;

	  spin_interupt_init();
	  spin_interupt_enable();

	  spin_timer_set(local_timer_current_used,mode2,5);
	  spin_interupt_open(local_timer_current_used,high);
		//开始计时
	  spin_timer_start();
}