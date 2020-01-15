#include"spin_interupt.h"
#include<stdlib.h>
#include<string.h>
uint* inter_vector_function[inter_num]={NULL,NULL,NULL,NULL,NULL};	//交给用户初始化
static uchar* inter_src_list[inter_num];
static uchar* inter_prority[inter_num];

//static void *callback_data;
void spin_interupt_init(){
	//中断源开关地址
	inter_src_list[out_int0]=0xA8;
	inter_src_list[timer0]=0xA9;
	inter_src_list[out_int1]=0xAA;
	inter_src_list[timer1]=0xAB;
	inter_src_list[serial]=0xAC;
	//中断优先级设置地址
	inter_prority[out_int0]=0xB8;
	inter_prority[timer0]=0xB9;
	inter_prority[out_int1]=0xBA;
	inter_prority[timer1]=0xBB;
	inter_prority[serial]=0xBC;
}

void spin_interupt_enable(){
	   EA=1;
}
void spin_interupt_disable(){
	   EA=0;
}
void spin_interupt_open(INTER_LIST num,INTER_PRORITY prority){
		IE |= 0x01<<num;
	   //*inter_prority[num] = prority;
	   //*inter_src_list[num] = enable;//按位寻址，可以写入1bit
}
void spin_interupt_close(INTER_LIST num){
		IE &= (~(0x01<<num));
	   //*inter_src_list[num] = disable;//按位寻址，可以写入1bit
}
void spin_interupt_set_params(INTER_LIST num,uchar params){

}

void intersvr0() interrupt 0{
	   if(inter_vector_function[0]!=NULL)
	   		((interupt_callback)inter_vector_function[0])();
}

void intersvr1(void) interrupt 1{
		if(inter_vector_function[1]!=NULL)
	   		((interupt_callback)inter_vector_function[1])();
}

void intersvr2(void) interrupt 2{
		if(inter_vector_function[2]!=NULL)
	  		 ((interupt_callback)inter_vector_function[2])();
}
void intersvr3(void) interrupt 3{
		if(inter_vector_function[3]!=NULL)
	   		((interupt_callback)inter_vector_function[3])();
}
extern sem_t uart_recv;
extern sem_t uart_send;		//发送信号量
extern uchar recv_buff[10];//接收buff
extern uchar recv_buff_ptr;
static uchar local_data=0;
void intersvr4(void) interrupt 4{
		if(RI==1){ //接收中断
			local_data = SBUF;
			recv_buff[recv_buff_ptr++] = local_data;
			RI=0;
			if(inter_vector_function[4]!=NULL)
	   		((interupt_callback)inter_vector_function[4])();
			sem_post(uart_recv);
		}
		if(TI==1){//发送中断
			TI=0;
			sem_post(uart_send);
			//todo
		}	
}
