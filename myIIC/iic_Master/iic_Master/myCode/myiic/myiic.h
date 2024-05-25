
#ifndef _MYIIC_H_
#define _MYIIC_H_

#include "main.h"

#define IIC_W 0
#define IIC_R 1 

//IO方向设置
#define SDA_IN()  {GPIOB->MODER&=~(3<<(4*2));GPIOB->MODER|=0<<4*2;}	//PB11输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(4*2));GPIOB->MODER|=1<<4*2;} //PB11输出模式
//IO操作
#define IIC_SCL   PBout(5) //SCL
#define IIC_SDA   PBout(4) //SDA
#define READ_SDA  PBin(4)  //输入SDA


void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_RecvACK(void);
void I2C_SendByte(uint8_t dat);
uint8_t I2C_RecvByte(uint8_t a);

#endif
