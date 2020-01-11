#ifndef MEM_H
#define MEM_H
#ifdef __cplusplus
extern "C"{
#endif
#define MEM_SIZE		256
#define MIN_ALLOC_BYTE	2	
#define BITMAP_SIZE (MEM_SIZE/(8*MIN_ALLOC_BYTE))
void *one_malloc(uint size);
void one_free(void *addr,uint size);
void one_memset(void *addr,uint value,uint size);
void one_memcpy(void *dest,void *src,uint size);
#ifdef __cplusplus
}
#endif
#endif