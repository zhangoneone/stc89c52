#ifndef SPINMEMORYMANAGEH
#define SPINMEMORYMANAGEH
#include "spin_common.h" 
#define memory_pool_size	60	  /*内存池大小 单位byte*/
#define memory_pool_table_size	10	 /*内存池分配表大小*/
extern uint memory_head;	   //init之后，内存池的首地址
bool memory_pool_init();
void* spin_memory_malloc(uchar size);
bool spin_memory_free(void* ptr);

#endif