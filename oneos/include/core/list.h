#ifndef LIST_H
#define LIST_H
#ifdef __cplusplus
extern "C"{
#endif
#include "arch/C51/arch.h"
typedef struct slist{
	void *ctx;
	struct slist*pre;
	struct slist*back;	
}slist,*splist,*spnode;

splist list_create();
spnode list_find(splist sp,spnode node);
err_t list_remove(splist sp,spnode node);
err_t list_delete(splist sp,spnode *node);
err_t list_add(splist sp,spnode node);
#ifdef __cplusplus
}
#endif
#endif