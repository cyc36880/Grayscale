/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-21 10:55:31
 * @LastEditTime: 2024-03-27 15:35:25
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Master\iic_Master\myCode\control\control.c
 */
#include "control.h"
#include "myiic.h"

#include "delay.h"

#include "Motor.h"
#include "myiic.h"

#include "stdio.h"

#define SERVER_ADDR 0x74

TypedefMotorDat GetData = {0};

uint8_t color_buf[20] = {0};

void setup(void)
{
	delay_init(168); //初始化延时
	
//	I2C_Start();
//	I2C_SendByte((SERVER_ADDR<<1)|IIC_W);
//	I2C_RecvACK();
//	I2C_SendByte(5);
//	I2C_RecvACK();
//	I2C_Stop();
//	HAL_Delay(5000);
}


void loop(void)
{
//	MotorSendCmd(MOTOR_ADDR, MOTOR_CMD_ROCONSPDG, 45, 0);
//	
	
	I2C_Start();
	I2C_SendByte((SERVER_ADDR<<1)|IIC_W);
	I2C_RecvACK();
	I2C_SendByte(2);
	I2C_RecvACK();
//	I2C_Stop();
	
	I2C_Start();
	I2C_SendByte((SERVER_ADDR<<1)|IIC_R);
	I2C_RecvACK();
	
	for(uint8_t i=0; i<5; i++)
	{
		color_buf[i] = I2C_RecvByte(1);
	}
	color_buf[5] = I2C_RecvByte(0);
	
	I2C_Stop();

	printf("%d %d %d %d %d %d\r\n", color_buf[0], color_buf[1], color_buf[2], color_buf[3], color_buf[4], color_buf[5]);
	delay_ms(200);
}


