/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-24 13:49:07
 * @LastEditTime: 2024-03-28 16:52:36
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\identify\BinVal.c
 */
#include "BinVal.h"

#include "Grayscale.h"

#include "stdio.h"

/**
 * 二值识别，只能识别 黑线 白底
 * 
 * 把二值识别 当成 灰度值识别的一种特殊情况处理
 * 
 * 
*/

BinVal_t BinValControl = 
{
    .studyFlag = 0,
    .Light = MAX_LIGHT,
}; //二值识别控制句柄

uint8_t bincolor[SENSORE_NUM] = {0};
// static uint8_t MaxBinC = 255, MinBinC = 0;
// static uint8_t MedianVal = 125; //不训练时，至少给个默认阈值



// 在flash中更新数据
void BinVal_Init(void)
{

}




/**
 * @description: 二值识别训练
 * @param {uint8_t} light 亮度（0-255）
 * @return {*}
 */
void study_BinVal(uint8_t light)
{
    study_Grayscale(light, light, light);
}


void color_Bin(uint8_t light)
{
    color_Grayscale(light, light, light);

    for(uint8_t i=0; i<SENSORE_NUM; i++)
    {
        bincolor[i] = GrayVal[i]>170 ? 1 : 0;
    }
}


