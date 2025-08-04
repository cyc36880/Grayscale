/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */
#include "myCode/myMain.h"
#include "myCode/inc/sys_config_and_flash.h"
#include "myCode/inc/register.h"

static const uint8_t I2C_SLAVEADDRESS_OTHER1[] = {
    0x71,
    0x72,
    0x73,
    0x74,
};


/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 222;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_ENABLE;
  hi2c1.Init.OwnAddress2 = 226;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_I2C1_ENABLE();

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

    /* I2C1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

// 初始化i2从c第二个地址
static void i2c_init(uint8_t sec_adddr)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = SERVER_ADDRESS << 1;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_ENABLE;
  hi2c1.Init.OwnAddress2 = sec_adddr << 1;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}
// flash 中读取iic地址,并初始化iic
void MAX_I2C_other_addr_changle(void)
{
  uint8_t i2c_slaveaddr_pos = 0;
  get_sys_config_data(SYSCONF_MEM_LIST_I2C_ADDR_OTHER_POS, &i2c_slaveaddr_pos);
  if (++i2c_slaveaddr_pos >= sizeof(I2C_SLAVEADDRESS_OTHER1)/sizeof(I2C_SLAVEADDRESS_OTHER1[0]))
      i2c_slaveaddr_pos = 0;
  set_sys_config_info(SYSCONF_MEM_LIST_I2C_ADDR_OTHER_POS, &i2c_slaveaddr_pos, 1);
  sync_sys_config_info();

  I2C_Slave_Reset(); //重新初始化iic
}
// 获取iic地址
uint8_t MAX_I2C_addr_get_other(void)
{
  uint8_t i2c_slaveaddr_pos = 0;
  get_sys_config_data(SYSCONF_MEM_LIST_I2C_ADDR_OTHER_POS, &i2c_slaveaddr_pos);
  return i2c_slaveaddr_pos;
}

// ========================================================
// ========================================================

//I2C引脚强制拉高，释放总线
static void I2C_Slave_Forced_UP()
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
void I2C_Slave_Reset(void)//初始化I2C从机接收态
{
	HAL_I2C_DeInit(&hi2c1);		// 注销I2C
	HAL_I2C_MspDeInit(&hi2c1);	// 注销引脚
	I2C_Slave_Forced_UP(); // I2C引脚强制拉高，释放总线

  i2c_init(I2C_SLAVEADDRESS_OTHER1[MAX_I2C_addr_get_other()]);//重新初始化iic
	HAL_I2C_EnableListen_IT(&hi2c1);
}


void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  UNUSED(hi2c);
  UNUSED(AddrMatchCode);
	// 发送数据
	if(TransferDirection == I2C_DIRECTION_RECEIVE)
	{
		HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, iic_read_reg.reg, iic_read_reg.size, I2C_FIRST_FRAME);
	}
	// 接收数据
	else if(TransferDirection == I2C_DIRECTION_TRANSMIT)
	{
    HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, iic_write_reg.reg, iic_write_reg.size, I2C_FIRST_FRAME);
	}
}


void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)  //监听中断回调
{
	HAL_I2C_EnableListen_IT(hi2c); // Restart
}


void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)  //全部发送完成回调
{
  UNUSED(hi2c);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)  //全部接收完成回调
{
  UNUSED(hi2c);
	iic_write_reg.changle_flag = 1; // 接收完成
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->ErrorCode & HAL_I2C_ERROR_AF)
  {
  }
  I2C_Slave_Reset();
}

/* USER CODE END 1 */
