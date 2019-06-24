#include"spin_uart.h"
#include"spin_timer.h"
#include"spin_interupt.h"
#include<stdio.h>
#include<string.h>
sem_t uart_send;
sem_t uart_recv;
uchar recv_buff[10];//接收buff
uchar recv_buff_ptr=0;
static const uint boaud_rate=2400;
static const uint over_time_us=13;//每over_time_us，定时器1溢出1次

uint spin_uart_init(){
	sem_init(uart_send,1);
	sem_init(uart_recv,0);
	spin_timer_for_uart(timer1,over_time_us);

   //SCON=0xF0;//方式3，多机通讯，允许接收
   //SCON = 0x50;		//8位数据,可变波特率
   SCON=0xD0;//方式3,允许接收 停止位1.5位
   PCON|=0x80;//52单片机支持。使能后，串口实际波特率加倍
   spin_interupt_open(serial);	//开串口中断
   return boaud_rate;
}

uint spin_write_uart(const char *buff,uint n){
	uint i = 0;
	for(;i<n;i++){
		sem_wait(uart_send);                                        //等待发送信号量
		SBUF=*buff;
		buff++;
	}
}
//阻塞方式
uchar spin_read_byte(){
	 char ret;
	 sem_wait(uart_recv);
	 critical_area_enter();
	 ret = recv_buff[--recv_buff_ptr];
	 critical_area_exit();
	 return ret;
}
//这个函数有问题
uint spin_read_line(char *buff){
	uint i = 0;
	for(;i<recv_buff_ptr;i++){
		sem_wait(uart_recv);
		critical_area_enter();
		buff[i] = recv_buff[--recv_buff_ptr];
		critical_area_exit();
		if(buff[i]==0x13)//回车键
			break;
	}
	buff[i++]='\0';
	recv_buff_ptr=0;//buff清空,这么处理有问题
	return i;//读到的长度
}
//读出所有的数据 非阻塞方式
uint spin_read_uart(char *buff){
	uint i = 0;
	critical_area_enter();
	for(;i<recv_buff_ptr;i++)
		buff[i] = recv_buff[i];
	recv_buff_ptr=0;   //buff清空
	sem_init(uart_recv,0);//信号量清空
	critical_area_exit();
	return i;//读到的长度	
}
