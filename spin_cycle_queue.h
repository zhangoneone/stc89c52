#ifndef SPINCYCLEQUEUEH
#define SPINCYCLEQUEUEH
#include"spin_common.h"
#include <reg52.h>
#include<stdlib.h>
#include<string.h>
typedef struct cycle_queue_t{
			uchar head,tail,cur_wrt,cur_rd;
			uchar *queue_content;
}cycle_queue_t;

#define cycle_queue_alloc(data_type,data_type_len,data_num)	 (data_type *)malloc(data_type_len*data_num)

uchar cycle_queue_init();
bool cycle_queue_push(uchar fd,uchar content);
uchar cycle_queue_pop(uchar fd);
//提供spin专用的接口
#define spin_cycle_queue_init()		 \
			cycle_queue_init()

#define spin_cycle_queue_push(fd,content)	\
			cycle_queue_push(fd,content)

#define spin_cycle_queue_pop(fd)	\
			cycle_queue_pop(fd)

#endif