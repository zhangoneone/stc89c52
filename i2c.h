#ifndef __I2C_H_
#define __I2C_H_

#include<reg52.h>
sbit SCL=P2^1;
sbit SDA=P2^0;

//模拟I2C总线的引脚定义
#define	 I2C_SCL 	SCL
#define	 I2C_SDA	SDA

//定义I2C总线时钟的延时值，要根据实际情况修改，取值1～255
//SCL信号周期约为(I2C_DELAY_VALUE*4+15)个机器周期
#define I2C_DELAY_VALUE		1 

//定义I2C总线停止后在下一次开始之前的等待时间，取值1～65535
//等待时间约为(I2C_STOP_WAIT_VALUE*8)个机器周期
//对于多数器件取值为1即可；但对于某些器件来说，较长的延时是必须的
#define I2C_STOP_WAIT_VALUE	1

//I2C总线初始化，使总线处于空闲状态

void I2C_Init(void);
void I2C_Start(void);
void I2C_Write(char dat);
char I2C_Read(void);
unsigned char I2C_GetAck(void);
void I2C_PutAck(unsigned char ack);
void I2C_Stop(void);

//I2C总线综合发送函数，向从机发送多个字节的数据
unsigned char I2C_Puts
(
	unsigned char SlaveAddr,
	unsigned int SubAddr,
	unsigned char SubMod,
	unsigned char *dat,
	unsigned int Size
);

//I2C总线综合接收函数，从从机接收多个字节的数据
unsigned char I2C_Gets
(
	unsigned char SlaveAddr,
	unsigned int SubAddr,
	unsigned char SubMod,
	unsigned char *dat,
	unsigned int Size
);

#endif	//_I2C_H_
