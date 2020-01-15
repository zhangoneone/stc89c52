#include"spin_exinterupt.h"

static uint* local_exint_dev0_callback;
static uint* local_exint_dev1_callback;

static uint spin_exint0_local_callback(){
	 //todo
	 inter_vector_function[out_int0] = local_exint_dev0_callback;
	 ((interupt_callback)inter_vector_function[out_int0])();
	 return spin_ok;
}

static uint spin_exint1_local_callback(){
	 //todo
	 inter_vector_function[out_int1] = local_exint_dev1_callback;
	 ((interupt_callback)inter_vector_function[out_int1])();
	  return spin_ok;
}

void spin_exint0_start(TRIGER_TYPES type){
	//修改中断回调函数入口
	local_exint_dev0_callback = inter_vector_function[out_int0];
	inter_vector_function[out_int0] = (uchar *)spin_exint0_local_callback;
	IT0 = type;
	spin_interupt_init();
	spin_interupt_enable();
	spin_interupt_open(out_int0,high);
}
void spin_exint1_start(TRIGER_TYPES type){
	local_exint_dev1_callback = inter_vector_function[out_int1];
	inter_vector_function[out_int1] = (uchar *)spin_exint1_local_callback;
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