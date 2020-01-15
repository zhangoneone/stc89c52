#include"spin_userstart.h"

//用户决定的标志位
void spin_set_F0(uchar value){
	F0 = !!value;//避免填入大于1的数
}
	   //rs1和rs0决定了使用的寄存器组
void spin_set_RS1(uchar value){
	 RS1  = !!value;
}
void spin_set_RS0(uchar value){
	 RS0  = !!value;
}
		   //range from 0~3
void spin_set_work_register_group(uchar index){
	 spin_set_RS0(index&0x01);
	 spin_set_RS1((index>>1)&0x01);
}
   //进位标志位
uchar spin_get_CY(){
	  return CY;
}
uchar spin_get_F0(){
	  return F0;
}
uchar spin_get_RS1(){
      return RS1;
}
uchar spin_get_RS0(){
	  return RS0;
}
//溢出标志位
uchar spin_get_OV(){
      return OV;
}
void spin_set_sp(uchar addr){
	  SP=addr;
}
uchar spin_get_sp(){
 	return SP;
}


