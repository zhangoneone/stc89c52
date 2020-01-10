#include"spin_watchdog.h"
#include<reg52.h>
void spin_watchdog_enable(){
	WDT_CONTR |=0x20;
}
void spin_watchdog_disable(){
	WDT_CONTR &=0xDF;
}
void spin_watchdog_idlework(){
	WDT_CONTR |=0x08;
}
//重新计数
void spin_watchdog_clear(){
	WDT_CONTR |=0x10;
}


uint spin_watchdog_overtime(uchar pre_scale,uchar mhz){
	 uchar pre_scale_value=0;
	 uint overtime=0; 
     WDT_CONTR &=0xf8;
	 switch(pre_scale){
	  case 2:pre_scale_value = 0;break;
	  case 4:pre_scale_value = 1;break;
	  case 8:pre_scale_value = 2;break;
	  case 16:pre_scale_value = 3;break;
	  case 32:pre_scale_value = 4;break;
	  case 64:pre_scale_value = 5;break;
	  case 128:pre_scale_value = 6;break;
	  case 256:pre_scale_value = 7;break;
	  default:break;
	 }
	 WDT_CONTR |= pre_scale_value;
	 overtime = pre_scale*393/mhz;
	 return overtime;  //单位ms
}  //返回看门狗溢出时间。mhz是系统频率