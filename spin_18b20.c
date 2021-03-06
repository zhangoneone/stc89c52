#include "spin_18b20.h"
#include"spin_timer.h"
#include"etimer.h"
#include"process.h"
#include"spin_timer.h"
#include"spin_1602.h"
#include<reg52.h>
#include<stdio.h>
/***************************************************************************************************    
工程名称：	18b20_1602
功能描述：	18B20传感器测温获取温度值储存在数组中。
硬件连接：  用1位杜邦线将J11_7与J17_1820连接，将1602液晶接口对应插接到P4接口。
维护记录：  2011-8-22
***************************************************************************************************/
sbit DQ=P3^7;    //温度控制口
//**************************************************************************************************
//传感器初始化
//**************************************************************************************************
init_18b20()
{
        uchar flag;
        DQ=1;
        delayus(30);  //延时
        DQ=0;
        delayus(600); //*延时，要求精度，要求大于480us*
        DQ=1;
        delayus(50); //*延时，要求精度，要求大于15us*
        flag=DQ;    //DQ管脚送出60-240us的0脉冲,以示初始化成功
        delayus(30);  //延时
}
//**************************************************************************************************
//写一个字节函数
//**************************************************************************************************
write_byte(uchar t)
{
 uchar i;
 for(i=0;i<8;i++)   //循环8次写入1字节
  {
    DQ=0;           //数据线置低
    delayus(30);      //延时
    DQ=t&0x01;      //发送1位数据,最低位开始
    delayus(300);      //*延时，要求精度*
    DQ=1;           //数据线置高
    t=t>>1;         //右移1位
  }
}
//**************************************************************************************************
//读一个字节函数
//**************************************************************************************************
uchar read_byte()
{
 uchar i,value=0;;
 for(i=0;i<8;i++)   //循环8次读取1字节
  {
    value=value>>1; //右移1位
    DQ=0;           //数据线置低
    delayus(30);      //延时
    DQ=1;           //数据线置高
    delayus(30);      //延时
    if(DQ==1)value=value|0x80;//判断接收的1位数据是否为1
    delayus(100);      //*延时，要求精度*
  }
 return(value);
}
//**************************************************************************************************
//float to ascii
//**************************************************************************************************
uchar *float_2_ascii(float dat,uchar *asc)
{
 if(dat<0){	         //符号位
     dat=-dat;
	 asc[4]=0xb0;
   }
 else{
 	 asc[4]=0x2b;  
   }
 dat*=10;              //本实验显示到小数点后1位,所以乘10，以便分离得到十分位

 asc[0]=((int)dat)%10+0x30;    //除10取余得温度十分位，1602只识别ASCII码，+0x30目的就是把16进制转ASCII
 asc[1]='.';		           //小数点
 asc[2]=((int)dat)%100/10+0x30;//除100取余得十位和个位，然后除10取整得温度个位
 asc[3]=((int)dat)/100+0x30;   //除100取整得温度十位
 return asc;
}
//**************************************************************************************************
//温度采集函数
//**************************************************************************************************
float get_temp()
{
    uint dat;
    uchar wenl,wenh;
    init_18b20();         //复位
    write_byte(0xcc);     //不进行编号匹配
    write_byte(0x44);     //进行温度转换
    init_18b20();         //复位   
    write_byte(0xcc);     //不进行编号匹配
    write_byte(0xbe);     //发读命令
    wenl=read_byte();     //温度低八位
    wenh=read_byte();     //温度高八位
    dat=(wenh<<8)+wenl;   //数据高低8位合并
	return (dat*0.0625+0.05);
}
PROCESS(_18b20,"_18b20");//温度采集
extern volatile process_data_t global_dataa;
PROCESS_THREAD(_18b20, ev, dataa)
{
	static struct etimer et;
	float cur_temp=0.0;
	uchar asc[5]={0,0,0,0,0};
    PROCESS_BEGIN();
	init_18b20();
	//延时5s的时钟
	etimer_set(&et,CLOCK_SECOND*6);
	while(1)
	{
		//等待1s
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		cur_temp=get_temp();
		float_2_ascii(cur_temp,asc);
		process_post(&lcd, lcd_update2, (void*)asc);
		etimer_restart(&et);
	}
   PROCESS_END();
}
