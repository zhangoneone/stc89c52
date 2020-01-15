#include"spin_exinterupt.h"
#include <reg52.h>	   //此文件中定义了51的一些特殊功能寄存器
void spin_exint0_start(TRIGER_TYPES type){
	IT0 = type;
	spin_interupt_init();
	spin_interupt_enable();
	spin_interupt_open(out_int0,high);
}
void spin_exint1_start(TRIGER_TYPES type){
	IT1 = type;
	spin_interupt_init();
	spin_interupt_enable();
	spin_interupt_open(out_int1,high);
}

void spin_exint0_stop(){
   spin_interupt_close(out_int0);
}

void spin_exint1_stop(){
   spin_interupt_close(out_int1);
}