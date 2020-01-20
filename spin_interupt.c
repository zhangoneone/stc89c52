#include"spin_interupt.h"
#include"spin_gpio.h"
uint inter_vector_function[inter_num]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};	//交给用户初始化
mutex_t it0,it1,it2,it3,it4,it5,it6,it7;
//code uchar* inter_src_list[inter_num]={0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xC2,0xC6};
//code uchar* inter_prority[inter_num]={0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xC3,0xC7};
 //设置信号量，把用户中断回调注册为task，当遇到了信号量，task执行
void spin_interupt_init(){
  it0=it1=it2=it3=it4=it5=it6=it7=0;
}
void spin_interupt_enable(){
	   EA=1;
}
void spin_interupt_disable(){
	   EA=0;
}
void spin_interupt_open(INTER_LIST num,INTER_PRORITY prority){
     if(out_int2 ==num){
		EX2=1;
		PX2=prority;
		}
	 else if(out_int3 == num) {
	 	EX3=1;
		PX3=prority;
		}
	 else {
		IE |= 0x01<<num;
		IP |= prority<<num;
		}
}
void spin_interupt_close(INTER_LIST num){
	if(out_int2 ==num)
		EX2=0;
	 else if(out_int3 == num)
	 	EX3=0;
	 else
	 	IE &= (~(0x01<<num));
}
//void intersvr0() interrupt 0{
//	   spin_set_gpio_bit_value(GPIO2,0,0);
//	   if(inter_vector_function[0]!=NULL)
//	   		mutex_unlock(it0);
//}
//static DATA uchar counter = 0; //每次timer0溢出是250us，4次才是1ms
//void intersvr1(void) interrupt 1{
//		counter++;
//		if(counter==4){
//			counter = 0;
//			if(inter_vector_function[1]!=NULL){
//	   			mutex_unlock(it1);
//				spin_set_gpio_bit_value(GPIO2,1,0);
//				((interupt_callback)inter_vector_function[1])(); //执行任务调度
//			}
//		}
//}

void intersvr2(void) interrupt 2{
//		spin_set_gpio_bit_value(GPIO2,2,0);
		if(inter_vector_function[2]!=NULL)
	  		 mutex_unlock(it2);
}
//void intersvr3(void) interrupt 3{
//		spin_set_gpio_bit_value(GPIO2,3,0);
//		if(inter_vector_function[3]!=NULL)
//	   		mutex_unlock(it3);
//}
//tim2中断
void intersvr5(void) interrupt 5{
//	spin_set_gpio_bit_value(GPIO2,5,0);
	if(inter_vector_function[5]!=NULL)
	   		mutex_unlock(it5);
}
//ext2中断
void intersvr6(void) interrupt 6{
//	spin_set_gpio_bit_value(GPIO2,6,0);
	if(inter_vector_function[6]!=NULL)
	   		mutex_unlock(it6);
}
//ext3中断
void intersvr7(void) interrupt 7{
//	spin_set_gpio_bit_value(GPIO2,7,0);
	if(inter_vector_function[7]!=NULL)
	   		mutex_unlock(it7);
}
