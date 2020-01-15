#ifndef SPINCOMMONH
#define SPINCOMMONH
#define C51
#ifdef C51
#define DATA data
#define IDATA idata
#define PDATA pdata
#define XDATA xdata
#define CODE code
#else
#define DATA 
#define IDATA 
#define PDATA 
#define XDATA 
#define CODE 
#endif
#define inter_num	8
#define NULL		0x00
#define FSOC		12 //单片机频率12MHZ
#define true		1
#define false		0
typedef unsigned char  uchar;   
typedef unsigned int   uint; 
typedef bit bool;   
typedef enum{
 GPIO0=0,
 GPIO1,
 GPIO2,
 GPIO3,
}GPIOS;
 /*
 P30 RXD 
 P31 TXD
 P32 out_int0
 P33 out_int1
 P34 timer0	  //外部脉冲计数
 P35 timer1	  //外部脉冲计数
 */
typedef enum{
 out_int0=0, 
 timer0,
 out_int1,
 timer1,
 serial,
 timer2,
 out_int2,
 out_int3,
}INTER_LIST;
typedef INTER_LIST devices;
typedef enum{
 low=0,
 high,
}INTER_PRORITY;

typedef enum{
   low_vol=0,
   down_eage,
}TRIGER_TYPES;
enum{
 disable=0,
 enable,
};
typedef enum{
   spin_ok,
   allocate_timer_fail,
   allocate_counter_fail,
}SPIN_STATUS;

typedef uchar sem_t;//计数型信号量
#define sem_init(sem_name,value)		sem_name = value
#define sem_wait(sem_name)				while(!sem_name);	\
										sem_name-=1	
#define sem_post(sem_name)				sem_name+=1

typedef bool mutex_t; //互斥量
#define mutex_init(mutex_name,value)	mutex_name = value
#define mutex_lock(mutex_name)			while(!mutex_name);	\
										mutex_name=0	
#define mutex_unlock(mutex_name)		mutex_name=1
//临界区 关闭全部中断

#define critical_area_enter()				spin_interupt_disable()
#define critical_area_exit()				spin_interupt_enable()
#endif