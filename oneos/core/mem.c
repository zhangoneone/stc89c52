#include "arch/C51/arch.h"
#include "core/mem.h"
#define USED 	1
#define UNUSED  0
static PDATA uchar mem_heap[MEM_SIZE];
static IDATA uchar bitmap[BITMAP_SIZE];
static void set_flag(uint offset,uint len,bool flag){
  uchar i=0;
  if(len%MIN_ALLOC_BYTE!=0)
  	len+=(MIN_ALLOC_BYTE-len%MIN_ALLOC_BYTE);
  len/=MIN_ALLOC_BYTE;
  i= (offset/MIN_ALLOC_BYTE)%8;
  offset/=(MIN_ALLOC_BYTE*8);
  for(;len>0;len--,i++){
  	if(i==8){
	  i=0;
	  offset++;
	}
	if(flag)
    	bitmap[offset]|=(0x01<<i);
	else 
		bitmap[offset]|=(0x00<<i);
  }
}

static void get_block(uint start_offset,uint *offset,uint *len,bool flag){
   uchar IDATA i=0,IDATA j=0;
   uint IDATA size=0;
   i=start_offset/(8*MIN_ALLOC_BYTE);
   j=start_offset%(8*MIN_ALLOC_BYTE);
   while(flag^(bitmap[i]&(0x1<<j))){
	 j++;
	 if(j==8){
	   j=0;
	   i++;
	 }
	 if(i==BITMAP_SIZE){*offset=0;*len=0;return;}
   }
   *offset=(i*8+j)*MIN_ALLOC_BYTE;
   while(!(flag^(bitmap[i]&(0x1<<j)))){
	 j++;
	 size++;
	 if(j==8){
	   j=0;
	   i++;
	 }
	 if(size*2>=(*len)){*len=size*2;return;}
   }
}
void *one_malloc(uint size){
	uint IDATA start=0,IDATA offset,IDATA len;
	len=size;
	for(;;){
	 get_block(start,&offset,&len,UNUSED);
	 if(size<=len)break;
	 start+=(offset+len);
	}
	set_flag(offset,size,USED);
	offset+=MIN_ALLOC_BYTE;//>>>>>
 	return (void *)mem_heap+offset;
}

void one_free(void *addr,uint size){
   set_flag(addr-mem_heap,size,UNUSED);
}

void one_memset(void *addr,uint value,uint size){
	if(addr==NULL)return;
	for(;size>0;size--){
	  *(((uchar*)addr)+size-1)=value;
	}
}

void one_memcpy(void *dest,void *src,uint size){
   if(dest==NULL||src==NULL)return;
   for(;size>0;size--){
	 *(((uchar*)dest)+size-1)=*(((uchar*)src)+size-1);
   }
}