#ifndef TASK_H
#define TASK_H
#ifdef __cplusplus
extern "C"{
#endif
#include "arch/C51/arch.h"
#include "core/list.h"
/*task status*/
#define RUN		0
#define BLOCK 	1
#define WAIT 	2
#define EXTINCT	3

typedef struct{
	uchar task_id;
	uchar status;
	void* stack_addr; /*task stack base address*/
	void* stack_p;/*point to stack top*/
	uchar stack_size;
	uchar priorty;	/*0~255 0 is min,255 is max*/
	void(*task_fun)(void);
}TASK,*TASKHANDLE;

typedef splist tasklist;
//#define system_tick()	(intersvr1(void) interrupt 1) 
TASKHANDLE task_create(uchar task_id,uchar priorty,uchar stack_size,void(*task_fun)(void));
err_t task_suspend(TASKHANDLE task);
err_t task_resume(TASKHANDLE task);
err_t task_delete(TASKHANDLE task);
err_t task_clean();
err_t schedule();
err_t oneos_init();
void oneos_start();
#ifdef __cplusplus
}
#endif
#endif