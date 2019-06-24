#include"spin_memory_manage.h"
static uint local_memory_head[default_memory_pool_size];
static bool prevent_bit=false;//∑¿÷π÷ÿ»Î
uint memory_head;
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
 	
}
bool  spin_memory_free(void*){

}