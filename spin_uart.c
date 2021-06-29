#include"spin_uart.h"
#include"spin_timer.h"
#include"spin_interupt.h"
#include<stdio.h>
#include<string.h>
volatile sem_t uart_send; //发送完成信号量
volatile buff_t idata recv_buff; //接收buff

static const uint boaud_rate=2400;
static const uint over_time_us=13;//每over_time_us，定时器1溢出1次

uint spin_uart_init(){
	sem_init(uart_send,1);
	spin_timer_for_uart(timer1,over_time_us);

   //SCON=0xF0;//方式3，多机通讯，允许接收
   //SCON = 0x50;		//8位数据,可变波特率
   SCON=0xD0;//方式3,允许接收 停止位1.5位
   PCON|=0x80;//52单片机支持。使能后，串口实际波特率加倍
   spin_interupt_open(serial);	//开串口中断
   return boaud_rate*2;
}
static uchar spin_write_byte(uchar c){
	sem_wait(uart_send);    //等待发送完成
	critical_area_enter();  //临界资源 SBUF
	SBUF=c;
	critical_area_exit();
	return c;	
}
uint spin_write_uart(const char idata *buff,uint n){
	uint i = 0;		
	for(;i<n;i++){
		spin_write_byte(*buff);
		buff++;
	}
	return i;
}
//返回0代表结束
static uchar spin_read_byte(){
	 char ret;
	 if(recv_buff.len<=0)return 0;
	 critical_area_enter();
	 ret = recv_buff.buff[--recv_buff.len];
	 critical_area_exit();
	 return ret;
}

uint spin_read_uart(char idata *buff){
	char c = 0;
	uint i = 0;
	uint j = 0;
	uint end = 0;
	while( (c=spin_read_byte()) !=0){
	  buff[i++] = c;
	}
	if(i<2){buff[i]='\0';return i;} 
//	//reverse array
	end = i-1;
	for(j=0;j<i/2;j++){
		c = buff[j];
		buff[j] = buff[end-j];
		buff[end-j] = c;
	}
	buff[i]='\0';
	return i;//读到的长度
}
