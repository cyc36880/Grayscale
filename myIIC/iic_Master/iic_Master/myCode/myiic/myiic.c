/*
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-01-18 13:34:25
 * @LastEditTime: 2024-01-25 17:12:24
 * @FilePath: \MDK-ARMd:\Desptop\icMatser_RTOS_guider_2\myCode\hardware\myiic\myiic.c
 */
#include "myiic.h"

#include "sys.h"
#include "delay.h"



//**************************************
//I2C起始信号
//**************************************
void I2C_Start(void)
{
	SDA_OUT();//sda线输出
    IIC_SDA = 1;                    //拉高数据线
    IIC_SCL = 1;                    //拉高时钟线
    delay_us(5);                 //延时
    IIC_SDA = 0;                    //产生下降沿
    delay_us(5);                 //延时
    IIC_SCL = 0;                    //拉低时钟线
}

//**************************************
//I2C停止信号
//**************************************
void I2C_Stop(void)
{
	SDA_OUT();//sda线输出
    IIC_SDA = 0;                    //拉低数据线
    IIC_SCL = 1;                    //拉高时钟线
    delay_us(5);                 //延时
    IIC_SDA = 1;                    //产生上升沿
    delay_us(5);                 //延时						   	
}
//**************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(uint8_t ack)
{
	SDA_OUT();//sda线输出
    IIC_SDA = ack;                  //写应答信号
    IIC_SCL = 1;                    //拉高时钟线
    delay_us(5);                 //延时
    IIC_SCL = 0;                    //拉低时钟线
    delay_us(5);                 //延时
}
//**************************************
//I2C接收应答信号 0应答
//**************************************
uint8_t I2C_RecvACK(void)
{
	uint8_t CY;
	
	SDA_IN();
    IIC_SCL = 1;                    //拉高时钟线
    delay_us(5);                 //延时
    CY = READ_SDA;                   //读应答信号
    IIC_SCL = 0;                    //拉低时钟线
    delay_us(5);                 //延时
    return CY;
}
//**************************************
//向I2C总线发送一个字节数据
//**************************************
void I2C_SendByte(uint8_t dat)
{
    uint8_t i;
	SDA_OUT();//sda线输出
    for (i=0; i<8; i++)         //8位计数器
    {
		IIC_SDA = (dat>>7);
        dat <<= 1;              //移出数据的最高位
        IIC_SCL = 1;                //拉高时钟线
        delay_us(5);             //延时
        IIC_SCL = 0;                //拉低时钟线
        delay_us(5);             //延时
    }	 
}
//**************************************
//从I2C总线接收一个字节数据
//**************************************
uint8_t I2C_RecvByte(uint8_t a)
{
    uint8_t i;
    uint8_t dat = 0;
    IIC_SDA = 1;                    //使能内部上拉,准备读取数据,
	SDA_IN();
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        IIC_SCL = 1;                //拉高时钟线
        delay_us(5);             //延时
        dat |= READ_SDA;             //读数据               
        IIC_SCL = 0;                //拉低时钟线
        delay_us(5);             //延时
    }
    I2C_SendACK(!a);
    return dat;
}
