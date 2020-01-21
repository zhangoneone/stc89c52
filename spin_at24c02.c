#include<reg52.h>
#include"i2c.h"
#include"spin_at24c02.h"
#include"etimer.h"
#include"process.h"	
#include"spin_1602.h"							 
/*******************************************************************************
* 函数名         : void At24c02Write(unsigned char addr,unsigned char dat)
* 函数功能		   : 往24c02的一个地址写入一个数据
* 输入           : 无
* 输出         	 : 0正常返回，1错误返回
*******************************************************************************/

char At24c02Write(unsigned char addr,unsigned char dat)
{
	return I2C_Puts(0xa0,addr,1,&dat,1);
}
/*******************************************************************************
* 函数名         : unsigned char At24c02Read(unsigned char addr)
* 函数功能		   : 读取24c02的一个地址的一个数据
* 输入           : 无
* 输出         	 : 错误返回是0xff
*******************************************************************************/

unsigned char At24c02Read(unsigned char addr)
{
	unsigned char num;
	if(I2C_Gets(0xa0,addr,1,&num,1)==0)return num;
	else return 0xff;
}
code char str[16]={'z','h','a','n','g','q','i','n','g','g','o','o','d','b','o','y'};
PROCESS(at24c02,"at24c02");
PROCESS_THREAD(at24c02, ev, dataa)
{
	static uchar slen=18;
	static uchar index=2;
	static uchar recv[16];
	static struct etimer et;
    PROCESS_BEGIN();
	I2C_Init();
	etimer_set(&et,CLOCK_SECOND/8);	
	while(1)
	{   
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));	
		if(At24c02Write(index,str[index]))break;
		index++;
		if(index==slen)break;
		etimer_restart(&et);
	}
	index=2;
	while(1)
	{   
		etimer_restart(&et);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));	
		recv[index-2] = At24c02Read(index);
		if(recv[index-2]==0xff)break;
		index++;
		if(index==slen)break;
	}
	process_post(&lcd, lcd_update, (void*)recv);
	etimer_set(&et,CLOCK_SECOND*65535);
	while(1){
	   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	   etimer_restart(&et);
	}
   PROCESS_END();
}