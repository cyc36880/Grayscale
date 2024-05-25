/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-24 13:49:07
 * @LastEditTime: 2024-03-28 16:52:36
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\identify\BinVal.c
 */
#include "BinVal.h"

#include "stdio.h"

/**
 * 二值识别，只能识别 黑线 白底
*/

BinVal_t BinValControl = 
{
    .studyFlag = 0,
    .Light = DEFAULT_LIGHT,
}; //二值识别控制句柄

uint8_t bincolor[SENSORE_NUM] = {0};
static uint8_t MaxBinC = 255, MinBinC = 0;
static uint8_t MedianVal = 125; //不训练时，至少给个默认阈值

/**
 * @description: 二值识别训练
 * @param {uint8_t} light 亮度（0-255）
 * @return {*}
 */
void study_BinVal(uint8_t light)
{
    MaxBinC = 0;   //学习时重置阈值
    MinBinC = 255;

    uint32_t tickstart = HAL_GetTick();
    uint32_t wait = 1000 * 5;
    if (wait < HAL_MAX_DELAY)
    {
        wait += (uint32_t)(uwTickFreq);
    }
    while ((HAL_GetTick() - tickstart) < wait) //
    {
        setLinght(light, light, light); //点亮
        HAL_Delay(1);//转换时长
        for (uint8_t i = 0; i < SENSORE_NUM; i++)
        {
            if (VAL4096_2_255(adcVal[i]) > MaxBinC)
                MaxBinC = VAL4096_2_255(adcVal[i]);
            if (VAL4096_2_255(adcVal[i]) < MinBinC)
                MinBinC = VAL4096_2_255(adcVal[i]);
        }
        HAL_Delay(15); //增加训练的视觉感受
        setLinght(0, 0, 0); //
        HAL_Delay(15); //增加训练的视觉感受
    }
    MedianVal = (MaxBinC + MinBinC) / 2; //计算中值
}


void color_Bin(uint8_t light)
{
    setLinght(light, light, light); //点亮
    HAL_Delay(1);
    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        if (VAL4096_2_255(adcVal[i]) > MedianVal)
            bincolor[i] = 0;
        else
            bincolor[i] = 1;
    }
    HAL_Delay(1);
//    printf("adc %d %d %d %d %d %d\r\n", adcVal[0], adcVal[1], adcVal[2], adcVal[3], adcVal[4], adcVal[5]);
//    printf("bin %d %d %d %d %d %d\r\n", bincolor[0], bincolor[1], bincolor[2], bincolor[3], bincolor[4], bincolor[5]);
}


