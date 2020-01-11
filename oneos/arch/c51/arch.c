#include "arch/C51/arch.h"
#include "core/task.h"
#include "start.h"
#include <reg52.h>
extern IDATA TASKHANDLE cur_task;
//当前task的上下文入栈 其中PC已经自动入栈了
void context_push(){
#pragma asm
  push 0
  push 1
  push 2
  push 3
  push 4
  push 5
  push 6
  push 7
  push 0xd0
  push ACC
  push 0xf0
  push 0x82
  push 0x83
#pragma endasm
 (cur_task->stack_p)=(void *)SP;
}
//当前上下文环境出栈
void context_pop(){
  SP=cur_task->stack_p;
#pragma asm
  pop 0x83
  pop 0x82
  pop 0xf0
  pop ACC
  pop 0xd0
  pop 7
  pop 6
  pop 5
  pop 4
  pop 3
  pop 2
  pop 1
  pop 0
#pragma endasm
}
