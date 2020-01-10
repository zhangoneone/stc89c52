#include"spin_cycle_queue.h"
code uchar SPIN_QUEUE_SIZE =20;
static cycle_queue_t xdata local_cycle_queue[1];
static bool xdata queue_index;
static uchar xdata local_queue_content[1][20];//SPIN_QUEUE_SIZE

static bool cycle_queue_full(cycle_queue_t *cycle_queue){
	   if(cycle_queue->cur_rd - cycle_queue->cur_wrt==1)
	   		return true;
	   else return false;
}

static bool cycle_queue_empty(cycle_queue_t *cycle_queue){
	   if(cycle_queue->cur_wrt == cycle_queue->cur_rd)
	   		return true;
	   else return false;
}
uchar cycle_queue_init(){
	  local_cycle_queue[queue_index].queue_content=local_queue_content[queue_index];
	  local_cycle_queue->head = 0;
	  local_cycle_queue->tail = SPIN_QUEUE_SIZE;
	  local_cycle_queue->cur_wrt = local_cycle_queue->cur_rd = 0;
	  return queue_index;
}

//先判断pop函数返回值，正常返回，content才有效
uchar cycle_queue_pop(uchar fd){
	 local_cycle_queue[fd].cur_rd%=local_cycle_queue[fd].tail;
	 if(cycle_queue_empty(local_cycle_queue+fd))
	 	return	false;
	 else
	 	return local_cycle_queue[fd].queue_content[local_cycle_queue[fd].cur_rd++];
}
bool cycle_queue_push(uchar fd,uchar content){
	 local_cycle_queue[fd].cur_wrt%= local_cycle_queue[fd].tail;
	 if(cycle_queue_full(local_cycle_queue+fd))
	 	return	false;
	 else
	 	local_cycle_queue[fd].queue_content[local_cycle_queue[fd].cur_wrt++] = content;
	content = local_cycle_queue[fd].queue_content[local_cycle_queue[fd].cur_wrt-1];
	return true;
}
