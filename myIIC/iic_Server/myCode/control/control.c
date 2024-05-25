/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-16 16:32:44
 * @LastEditTime: 2024-04-03 17:39:31
 * @FilePath: \MDK-ARMd:\Desktop\iic_Server\myCode\control\control.c
 */
#include "control.h"

#include <stdio.h>

#include "adc.h"
#include "tim.h"
#include "i2c.h"

#include "Protocol.h"
#include "flash.h"

#include "LED.h"

#include "color.h"
#include "Grayscale.h"
#include "BinVal.h"


static uint8_t NoDateBuf[6] = {0}; //不在3模式下时，主机读到的数据


lv_color_hsv_t Light_HSV[SENSORE_NUM] = {0}; //hsv
lv_color_rgb_t Light_RGB[SENSORE_NUM] = {0}; //rgb
uint32_t adcVal[SENSORE_NUM] = {0}; //adc值

Control_t control = {0}; //控制句柄

static void TXTragetBuf_Select(uint8_t sw);

static void Control_Init(void);
static void Control_Run(void);
static void KeyControl(void);


void setup(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	
	HAL_ADCEx_Calibration_Start(&hadc1); //校准
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcVal, SENSORE_NUM); //开启adc 

	LED_Init(); 
	Control_Init();
	ServerI2C_Init(); //初始化从机iic
	color_init();
}


void loop(void)
{
	LED_Run();
	Control_Run();
	KeyControl();
}


/**
 * @description: 三色灯亮度
 * @param {uint8_t} r 红色 （0-255）
 * @param {uint8_t} g 绿色 （0-255）
 * @param {uint8_t} b 蓝色 （0-255）
 * @return {*}
 */
void setLinght(uint8_t r, uint8_t g, uint8_t b)
{
    TIM1->CCR1 = r;
    TIM1->CCR2 = g;
    TIM1->CCR3 = b;
}


static void Control_Init(void)
{
	TXTragetBuf_Select(control.sw);
	
}


static void KeyControl(void)
{
	if(HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET)
	{

		HAL_Delay(10);
		if(HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET)
		{
			uint32_t count = 0;
			while(HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET)
			{
				HAL_Delay(10);
				if(count < 100 * 3) count++;
			}
			
			if(count < 100 * 3) // 短按
			{
				if(control.sw == 0) // 无模式，按键切换到二值训练识别
				{
					control.sw = 5;
				}
				else if( control.sw== 2) {control.sw = 4;}
				else if( control.sw== 3) {control.sw = 5;}
				control_iic_rx_CpltCallback(); //按键模拟iic回调函数的执行流程
			}
			else
			{
				if(++SecAlaveAddrPos >= 4) SecAlaveAddrPos=0;
				Write_Flash_Data(SecAlaveAddrPos);
				I2C_Slave_Rest();
			}
		}
	}
}




static void Control_Run(void)
{
	TXTragetBuf_Select(control.sw);
	
	{ //学习 (学习后开始识别)
		if(control.sw==4 && GrayControl.studyFlag == 1) //灰度学习
		{
			LED_SET_ALL(1);
		    GrayControl.studyFlag = 0;
			study_Grayscale(GrayControl.rLight, GrayControl.gLight, GrayControl.bLight);
			control.sw = 2;
		}
		if(control.sw==5 && BinValControl.studyFlag == 1) //二值化学习
		{
			LED_SET_ALL(1);
		    BinValControl.studyFlag = 0;
			study_BinVal(BinValControl.Light);
			control.sw = 3;
		}
	}

	{ //识别
		if(control.sw == 1) //颜色识别
		{
			Color_Recongnition(colorControl.rLight, colorControl.gLight, colorControl.bLight);
		}
		else if(control.sw == 2) //灰度
		{
			color_Grayscale(GrayControl.rLight, GrayControl.gLight, GrayControl.bLight);
		}
		else if(control.sw == 3) //二值化
		{
			color_Bin(BinValControl.Light);
		}
	}
}


/**
 * @description: 数据发送目标选择
 * @param {uint8_t} sw
 * @return {*}
 */
static void TXTragetBuf_Select(uint8_t sw)
{
	switch(sw)
	{
	  case 1:
			control.TXBuf = color;
			break;
		case 2:
			control.TXBuf = GrayVal;
			break;
		case 3:
			control.TXBuf = bincolor;
			break;
		default :
			control.TXBuf = NoDateBuf;
			break;
	}
}

void control_iic_tx_CpltCallback(void)
{
	// printf("sw:%d %d %d %d %d %d %d \r\n", control.sw, control.TXBuf[0], control.TXBuf[1], control.TXBuf[2], control.TXBuf[3], control.TXBuf[4], control.TXBuf[5]);
}
void control_iic_rx_CpltCallback(void)
{
	if(control.sw == 4) //灰度
	{
		GrayControl.studyFlag = 1; //灰度学习
	}
	else if(control.sw == 5) //二值化
	{
	    BinValControl.studyFlag = 1; //二值化学习
	}
	else
	{
		
	}
}






