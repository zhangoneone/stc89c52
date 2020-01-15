#include"spin_uart.h"
#include"spin_timer.h"
#include"spin_interupt.h"
#include"spin_gpio.h"
#include<reg52.h>

//static const uint boaud_rate=2400;
//static const uint over_time_us=13;//每over_time_us，定时器1溢出1次

uint spin_uart_init(){
	//SCON=0xF0;//方式3，多机通讯，允许接收
    //SCON = 0x50;		//8位数据,可变波特率
    SCON=0xD0;//方式3,允许接收 停止位1.5位
    PCON|=0x80;//52单片机支持。使能后，串口实际波特率加倍
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	uart_boaud(13);
   spin_interupt_open(serial,high);	//开串口中断
   return 4800;
}
IDATA volatile struct{
	  uchar twi,tri,tcnt;
	  uchar rwi,rri,rcnt;
	  uchar tbuf[SEBULEN];
	  uchar rbuf[REBULEN];
}fifo;

 //阻塞读
uchar u_getc(){
   	uchar d;
	uchar i;

	/* Wait while rx fifo is empty */
	while (!fifo.rcnt);

	i = fifo.rri;			/* Get a byte from rx fifo */
	d = fifo.rbuf[i];
	fifo.rri = ++i % REBULEN;
	fifo.rcnt--;
	return d;
}
//这个一般不会阻塞
void u_putc(uchar c){
   	uchar i;
	/* Wait for tx fifo is not full */
	while (fifo.tcnt >= SEBULEN) ;

	i = fifo.twi;		/* Put a byte into Tx fifo */
	fifo.tbuf[i] = c;
	fifo.twi = ++i % SEBULEN;
	fifo.tcnt++;
	TI=1;//使能发送中断
}

void intersvr4(void) interrupt 4{
    uchar i;
	uchar d;
    if(RI==1){ //接收中断
		RI=0;
		d = SBUF;
		i = fifo.rcnt;
		if (i < REBULEN) {	/* Store it into the rx fifo if not full */
		fifo.rcnt = ++i;
		i = fifo.rwi;
		fifo.rbuf[i] = d;
		fifo.rwi = ++i % REBULEN;
		}
	}
	if(TI==1){//发送中断
		i = fifo.tcnt;
		if (i--) {	/* There is any data in the tx fifo */
			fifo.tcnt = i;
			i = fifo.tri;
			SBUF = fifo.tbuf[i];
			fifo.tri = ++i % SEBULEN;
		} else {	/* No data in the tx fifo */
			TI=0;
		}
	}	
}