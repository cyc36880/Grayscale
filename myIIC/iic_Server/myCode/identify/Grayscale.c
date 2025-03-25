/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 15:38:09
 * @LastEditTime: 2024-03-29 13:34:44
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\identify\Grayscale.c
 */
#include "Grayscale.h"

#include "flash.h"

#include "hsv.h"
#include "color.h"
#include "stdlib.h"

/**
 * 要求：要循的线为黑色（最好为白底）
*/

#if 0
uint8_t bgColor = 8; // 背景色 （颜色索引）
uint8_t fgColor = 9; // 前景色 （颜色索引）

uint8_t bgGray = 0; // 背景色 （灰度值）
uint8_t fgGray = 0; // 前景色 （灰度值）

static uint8_t invert = 0;
static uint8_t Grayscale[SENSORE_NUM] = {0};

#endif

Grayscale_t GrayControl = 
{
    .studyFlag=0,
    .rLight = 255,
    .gLight = MAX_LIGHT,
    .bLight = MAX_LIGHT,
}; //灰度控制句柄


// uint8_t maxVal=255, minVal=0;
// int16_t MedianVal = 127;
// static float zoom = 1.0;
// static uint8_t offset = 0;

Grayscale_Port Grayscale_Port_param[6] = {
    {.maxVal=4095, .minVal=0, .MedianVal=2047, .zoom=1.0, .offset=0},
    {.maxVal=4095, .minVal=0, .MedianVal=2047, .zoom=1.0, .offset=0},
    {.maxVal=4095, .minVal=0, .MedianVal=2047, .zoom=1.0, .offset=0},
    {.maxVal=4095, .minVal=0, .MedianVal=2047, .zoom=1.0, .offset=0},
    {.maxVal=4095, .minVal=0, .MedianVal=2047, .zoom=1.0, .offset=0},
    {.maxVal=4095, .minVal=0, .MedianVal=2047, .zoom=1.0, .offset=0},
};



static int32_t Grayscale[SENSORE_NUM] = {0};
// 识别后的n路灰度值
uint8_t GrayVal[SENSORE_NUM] = {0};




// 从flash中读取灰度学习数据
void Grayscale_Init(void)
{
    uint8_t *bufStart = (uint8_t *) (get_flash_buf() + 1);

    for(uint8_t i=0; i<SENSORE_NUM; i++)
    {
        bufStart += sizeof(Grayscale_Port);
        Grayscale_Port_param[i].maxVal = ((Grayscale_Port *)bufStart)->maxVal;
        Grayscale_Port_param[i].minVal = ((Grayscale_Port *)bufStart)->minVal;
        Grayscale_Port_param[i].MedianVal = ((Grayscale_Port *)bufStart)->MedianVal;
        Grayscale_Port_param[i].offset = ((Grayscale_Port *)bufStart)->offset;
        Grayscale_Port_param[i].zoom = ((Grayscale_Port *)bufStart)->zoom;
    }
}

//  写灰度学习的值
static void update_Grayscale_flashdata(void)
{
    uint8_t *bufStart = (uint8_t *) (get_flash_buf() + 1);

    for(uint8_t i=0; i<SENSORE_NUM; i++)
    {
        bufStart += sizeof(Grayscale_Port);

        ((Grayscale_Port *)bufStart)->maxVal = Grayscale_Port_param[i].maxVal;
        ((Grayscale_Port *)bufStart)->minVal =  Grayscale_Port_param[i].minVal;
        ((Grayscale_Port *)bufStart)->MedianVal = Grayscale_Port_param[i].MedianVal;
        ((Grayscale_Port *)bufStart)->offset = Grayscale_Port_param[i].offset;
        ((Grayscale_Port *)bufStart)->zoom =Grayscale_Port_param[i].zoom;
    }
    updata_flash();
}




/**
 * @description: 灰度学习（死循环学习）
 * @param {uint8_t} r
 * @param {uint8_t} g
 * @param {uint8_t} b
 * @return {*}
 */
void study_Grayscale(uint8_t r, uint8_t g, uint8_t b)
{
    for(uint8_t i=0; i<SENSORE_NUM; i++)
    {
        Grayscale_Port_param[i].maxVal = 0;
        Grayscale_Port_param[i].minVal = 4095;
    }

    uint32_t tickstart = HAL_GetTick();
    uint32_t wait = 1000 * 5;
    if (wait < HAL_MAX_DELAY)
    {
        wait += (uint32_t)(uwTickFreq);
    }
    while ((HAL_GetTick() - tickstart) < wait) //
    {
        setLinght(r, g, b);
        HAL_Delay(1);
        for(uint8_t i = 0; i < SENSORE_NUM; i++)
        {
            if (adcVal[i] > Grayscale_Port_param[i].maxVal)
                Grayscale_Port_param[i].maxVal = adcVal[i];
            if ((adcVal[i])< Grayscale_Port_param[i].minVal)
                Grayscale_Port_param[i].minVal = adcVal[i];
        }

        HAL_Delay(15); //增加训练的视觉感受
        setLinght(0, 0, 0); //
        HAL_Delay(15); //增加训练的视觉感受
    }
    for(uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        Grayscale_Port_param[i].zoom = 4095.0f / (Grayscale_Port_param[i].maxVal - Grayscale_Port_param[i].minVal); //计算缩放比例
        Grayscale_Port_param[i].MedianVal = (Grayscale_Port_param[i].maxVal + Grayscale_Port_param[i].minVal) / 2; // 计算中值
        Grayscale_Port_param[i].offset = Grayscale_Port_param[i].minVal;
    }

    update_Grayscale_flashdata(); //向flash写入数据
}



void color_Grayscale(uint8_t r, uint8_t g, uint8_t b)
{
    setLinght(r, g, b);
    HAL_Delay(1);//adc转换时间
    for(uint8_t i = 0; i < SENSORE_NUM; i++) //获取原始值
    {
        Grayscale[i] = adcVal[i];   
    }

    for(uint8_t i = 0; i < SENSORE_NUM; i++) 
    {
        Grayscale[i] +=  (Grayscale_Port_param[i].MedianVal - Grayscale[i])*2;//围绕中值反转
        Grayscale[i] = (Grayscale[i] - Grayscale_Port_param[i].offset) * Grayscale_Port_param[i].zoom; //比例缩放
    }
    
    for(uint8_t i = 0; i < SENSORE_NUM; i++) // 输出灰度值
    {
        if(Grayscale[i] < 0) Grayscale[i] = 0;
        Grayscale[i] = VAL4096_2_255(Grayscale[i]);

        Grayscale[i] = Grayscale[i]<=255 ? Grayscale[i] : 255;//放大限幅
        Grayscale[i] = Grayscale[i]>=0 ? Grayscale[i] : 0;
        GrayVal[i] = Grayscale[i];
    }
    // printf("max:%d, min:%d, \n",  maxVal, minVal);
    // printf("%3d %3d %3d %3d %3d %3d \n", GrayVal[0], GrayVal[1], GrayVal[2], GrayVal[3], GrayVal[4], GrayVal[5]);
}

