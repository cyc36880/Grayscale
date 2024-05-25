/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-28 16:12:58
 * @LastEditTime: 2024-03-29 13:53:09
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\hardware\LED\LED.c
 */
/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-28 16:12:58
 * @LastEditTime: 2024-03-28 16:34:28
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\hardware\LED\LED.c
 */
#include "LED.h"

#include "sys.h"
#include "control.h"

#include "Grayscale.h"
#include "BinVal.h"

// 设置led状态 x=1亮,x=0灭 （倒置）
#define SetLEDSta(x) ((x)==1 ? GPIO_PIN_SET : GPIO_PIN_RESET)

struct LED_PIN
{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pinx;
};

static const struct LED_PIN LED_PIN_X[] = //LED灯的端口和引脚
{
    {LED1_GPIO_Port, LED1_Pin},
    {LED2_GPIO_Port, LED2_Pin},
    {LED3_GPIO_Port, LED3_Pin},
    {LED4_GPIO_Port, LED4_Pin},
    {LED5_GPIO_Port, LED5_Pin},
    {LED6_GPIO_Port, LED6_Pin},
};


void LED_Init(void)
{
    LED_SET_ALL(0);
}

void LED_Run(void)
{
    if(control.sw == SW_OFF) LED_SET_ALL(0);
    else if(control.sw == SW_GRAY) //灰度模式
    {
        for(uint8_t i=0;i<sizeof(LED_PIN_X)/sizeof(LED_PIN_X[0]);i++)
        {
            HAL_GPIO_WritePin(LED_PIN_X[i].GPIOx, LED_PIN_X[i].GPIO_Pinx, SetLEDSta(GrayVal[i] > 125) );
        }
    }
    else if(control.sw == SW_BINARY) //二值模式
    {
        for(uint8_t i=0;i<sizeof(LED_PIN_X)/sizeof(LED_PIN_X[0]);i++)
        {
            HAL_GPIO_WritePin(LED_PIN_X[i].GPIOx, LED_PIN_X[i].GPIO_Pinx, SetLEDSta(bincolor[i]));
        }
    }
    else if(control.sw == SW_COLOR)
    {
        LED_SET_ALL(0);
    }
}


/**
 * @description: 设置所有的led状态
 * @param {uint8_t} sta 1亮 0灭
 * @return {*}
 */
void LED_SET_ALL(uint8_t sta)
{
    for(uint8_t i=0;i<sizeof(LED_PIN_X)/sizeof(LED_PIN_X[0]);i++)
    {
        HAL_GPIO_WritePin(LED_PIN_X[i].GPIOx, LED_PIN_X[i].GPIO_Pinx, SetLEDSta(sta));
    }
}



