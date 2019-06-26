//=========================================简易任务调度器==============================================
#define  TASK_MAX   3		 //任务数

typedef struct Tasks
{
    unsigned char TaskNumber; //任务的编号 
	unsigned char Run;    // 0表示任务不运行，1表示任务运行
    unsigned int Timer;   // 任务执行间隔时间
    unsigned int ItvTime; // 数值上等于Timer
    void (*TaskHook)(void); //任务函数
}Tasks;  
void TaskHangup(unsigned char Task_Num);
void TaskRecovery(unsigned char Task_Num);
void TaskProcess(void);
void TaskRemarks(void);
