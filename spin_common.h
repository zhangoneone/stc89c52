#ifndef SPINCOMMONH
#define  SPINCOMMONH
#define inter_num	5
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
}INTER_LIST;

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

typedef struct buff_t{
	char buff[10];	//接收缓冲区
	uchar len;
}buff_t;
typedef uint sem_t;		//计数型信号量，负责缓冲区的full or empty
#define sem_init(sem_name,value)		sem_name = value
#define sem_wait(sem_name)				while(!sem_name);	\
										sem_name-=1	
#define sem_post(sem_name)				sem_name+=1

typedef uint mutex_t;  //负责缓冲区的读写互斥
#define mutex_init(mutex_name)			mutex_name = 1
#define mutex_lock(mutex_name)			while(!mutex_name);	\
										mutex_name=0	
#define mutex_unlock(mutex_name)		mutex_name=1
//临界区 注意，目前临界区只有串口需要使用，故这样设置
//#define critical_area_enter()				spin_interupt_close(serial)
//#define critical_area_exit()				spin_interupt_open(serial,high)

#define critical_area_enter()				spin_interupt_disable()
#define critical_area_exit()				spin_interupt_enable()
#endif