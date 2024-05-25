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
//I2C��ʼ�ź�
//**************************************
void I2C_Start(void)
{
	SDA_OUT();//sda�����
    IIC_SDA = 1;                    //����������
    IIC_SCL = 1;                    //����ʱ����
    delay_us(5);                 //��ʱ
    IIC_SDA = 0;                    //�����½���
    delay_us(5);                 //��ʱ
    IIC_SCL = 0;                    //����ʱ����
}

//**************************************
//I2Cֹͣ�ź�
//**************************************
void I2C_Stop(void)
{
	SDA_OUT();//sda�����
    IIC_SDA = 0;                    //����������
    IIC_SCL = 1;                    //����ʱ����
    delay_us(5);                 //��ʱ
    IIC_SDA = 1;                    //����������
    delay_us(5);                 //��ʱ						   	
}
//**************************************
//I2C����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(uint8_t ack)
{
	SDA_OUT();//sda�����
    IIC_SDA = ack;                  //дӦ���ź�
    IIC_SCL = 1;                    //����ʱ����
    delay_us(5);                 //��ʱ
    IIC_SCL = 0;                    //����ʱ����
    delay_us(5);                 //��ʱ
}
//**************************************
//I2C����Ӧ���ź� 0Ӧ��
//**************************************
uint8_t I2C_RecvACK(void)
{
	uint8_t CY;
	
	SDA_IN();
    IIC_SCL = 1;                    //����ʱ����
    delay_us(5);                 //��ʱ
    CY = READ_SDA;                   //��Ӧ���ź�
    IIC_SCL = 0;                    //����ʱ����
    delay_us(5);                 //��ʱ
    return CY;
}
//**************************************
//��I2C���߷���һ���ֽ�����
//**************************************
void I2C_SendByte(uint8_t dat)
{
    uint8_t i;
	SDA_OUT();//sda�����
    for (i=0; i<8; i++)         //8λ������
    {
		IIC_SDA = (dat>>7);
        dat <<= 1;              //�Ƴ����ݵ����λ
        IIC_SCL = 1;                //����ʱ����
        delay_us(5);             //��ʱ
        IIC_SCL = 0;                //����ʱ����
        delay_us(5);             //��ʱ
    }	 
}
//**************************************
//��I2C���߽���һ���ֽ�����
//**************************************
uint8_t I2C_RecvByte(uint8_t a)
{
    uint8_t i;
    uint8_t dat = 0;
    IIC_SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
	SDA_IN();
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        IIC_SCL = 1;                //����ʱ����
        delay_us(5);             //��ʱ
        dat |= READ_SDA;             //������               
        IIC_SCL = 0;                //����ʱ����
        delay_us(5);             //��ʱ
    }
    I2C_SendACK(!a);
    return dat;
}
