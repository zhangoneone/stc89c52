#include"spin_memory_manage.h"
#include <stdlib.h>
#include <stdio.h>
#define default_memory_pool_size	20
static xdata uint local_memory_head[memory_pool_size];
static xdata bool prevent_bit=false;//防止重入
uint memory_head;	   //init之后，内存池的首地址
static xdata struct mem_table_t{
	uchar offset;
	uchar size;
	uchar  used;
}mem_table[memory_pool_table_size];//内存分配表

bool memory_pool_init(uchar size){
	 if(prevent_bit==true)
	 	return true;
 	 if(size==NULL)
		size = default_memory_pool_size;
	 memory_head = (uint)local_memory_head;
	 if(memory_head==0)return false;
	 prevent_bit = true;
	 return true;
}

void* spin_memory_malloc(uchar size){
     uchar i = 0;
 	 for(;i<memory_pool_table_size;i++){
		 if(mem_table[i].used ==false)
		    break;
	 }
	 if(i == memory_pool_table_size)
	    return NULL;
	 mem_table[i].used = true;
	 mem_table[i].offset = (i == 0?0:(mem_table[i-1].offset + mem_table[i-1].size));
	 mem_table[i].size = size;
	 return &mem_table[i];
}
bool spin_memory_free(void* ptr){
     uchar i = 0;
	 for(;i<memory_pool_table_size;i++){
		 if(&mem_table[i] == ptr)
		    break;
	 }
	 if(i == memory_pool_table_size)
	    return false;
	 else
	    mem_table[i].used = false;
}