/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 09:33:03
 * @LastEditTime: 2024-04-03 17:21:41
 * @FilePath: \MDK-ARMd:\Desktop\iic_Server\myCode\control\Protocol .c
 */
#include "Protocol.h"

#include "i2c.h"

#include "control.h"
#include "color.h"

#include "stdio.h"

#include "flash.h"

const uint8_t SecAlaveAddrArray[4] = {0x71, 0x72, 0x73, 0x74}; //需要切换的地址
uint8_t SecAlaveAddrPos = 0;




void ServerI2C_Init(void)
{
	uint16_t dat = Get_Flash_Data();
	if(dat >= 4) SecAlaveAddrPos = 0;
	else SecAlaveAddrPos = dat;
	
	I2C_Slave_Rest();
	HAL_I2C_EnableListen_IT(&hi2c1); //打开i2c监听
}


//I2C引脚强制拉高，释放总线
void I2C_Slave_Forced_UP()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
//	PB8     ------> I2C1_SCL
//	PB9     ------> I2C1_SDA
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);
}


/**
 * @description: iic意外，重新初始化iic
 * @return {*}
 */
void I2C_Slave_Rest(void)//初始化I2C从机接收态
{
	HAL_I2C_DeInit(&hi2c1);		// 注销I2C
	HAL_I2C_MspDeInit(&hi2c1);	// 注销引脚
	I2C_Slave_Forced_UP(); // I2C引脚强制拉高，释放总线

	MX_I2C1_Init(); //重新初始化iic
	
	HAL_I2C_EnableListen_IT(&hi2c1);
}


void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	// 发送数据
	if(TransferDirection == I2C_DIRECTION_RECEIVE)
	{
		HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, control.TXBuf, 6, I2C_FIRST_FRAME);
	}
	// 接收数据
	else if(TransferDirection == I2C_DIRECTION_TRANSMIT)
	{
		HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, &control.sw, 1, I2C_FIRST_FRAME);
	}
}


void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)  //监听中断回调
{
	HAL_I2C_EnableListen_IT(hi2c); // Restart
}


void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)  //全部发送完成回调
{
	control_iic_tx_CpltCallback();
}
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)  //全部接收完成回调
{
	control_iic_rx_CpltCallback();
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
//	printf("I2C Error\r\n");
	I2C_Slave_Rest();
}

