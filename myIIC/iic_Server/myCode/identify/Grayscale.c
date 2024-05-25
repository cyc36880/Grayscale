/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 15:38:09
 * @LastEditTime: 2024-03-29 13:34:44
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\identify\Grayscale.c
 */
#include "Grayscale.h"

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
    .rLight = DEFAULT_LIGHT,
    .gLight = DEFAULT_LIGHT,
    .bLight = DEFAULT_LIGHT,
}; //灰度控制句柄


static uint8_t maxVal=255, minVal=0;//反过来定义，是为了一定能更新数据
static int16_t MedianVal = 127;
static float zoom = 1.0;
static uint8_t offset = 0;

static int16_t Grayscale[SENSORE_NUM] = {0};
// 识别后的n路灰度值
uint8_t GrayVal[SENSORE_NUM] = {0};




//const lv_color_hsv_t color_gry_table[] = {
//    // 色表
//    HSV_RED,
//    HSV_ORANGE,
//    HSV_YELLOW,
//    HSV_GREEN,
//    HSV_CYAN,
//    HSV_BLUE,
//    HSV_PURPLE,
//    HSV_WHITE,
//    HSV_BLACK,
//};



static uint8_t rgb2gray(uint8_t r, uint8_t g, uint8_t b);






/**
 * @description: 灰度学习（死循环学习）
 * @param {uint8_t} r
 * @param {uint8_t} g
 * @param {uint8_t} b
 * @return {*}
 */
void study_Grayscale(uint8_t r, uint8_t g, uint8_t b)
{
#if 0 //rgb -> hsv -> gray
    uint8_t max_Grayscale = 0;
    uint8_t min_Grayscale = 255;
    uint8_t Grayvalue = 0;

    uint32_t tickstart = HAL_GetTick();
    uint32_t wait = 1000 * 5;
    if (wait < HAL_MAX_DELAY)
    {
        wait += (uint32_t)(uwTickFreq);
    }
    while ((HAL_GetTick() - tickstart) < wait) //
    {
        rgb2hsv(r, g, b); // 主要为了获取 rgb 值

        for (uint8_t i = 0; i < SENSORE_NUM; i++)
        {
            Grayvalue = rgb2gray(Light_RGB[i].r, Light_RGB[i].g, Light_RGB[i].b);

            if (Grayvalue > max_Grayscale)
                max_Grayscale = Grayvalue;
            if (Grayvalue < min_Grayscale)
                min_Grayscale = Grayvalue;
        }
        HAL_Delay(17);
    }
    // 计算前景 与 背景 的灰度值
    uint8_t cr, cg, cb;

    lv_color_hsv_to_rgb(color_gry_table[bgColor - 1].h, color_gry_table[bgColor - 1].s, color_gry_table[bgColor - 1].v, &cr, &cg, &cb);
    bgGray = rgb2gray(cr, cg, cb);

    lv_color_hsv_to_rgb(color_gry_table[fgColor - 1].h, color_gry_table[fgColor - 1].s, color_gry_table[fgColor - 1].v, &cr, &cg, &cb);
    fgGray = rgb2gray(cr, cg, cb);

    if (abs(max_Grayscale - fgGray) > abs(max_Grayscale - bgGray))
        invert = 1; // 确定前景与背景
#endif

#if 1 //adc -> gra 
	maxVal = 0, minVal = 255;
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
            if (VAL4096_2_255(adcVal[i]) > maxVal)
                maxVal = VAL4096_2_255(adcVal[i]);
            if (VAL4096_2_255(adcVal[i]) < minVal)
                minVal = VAL4096_2_255(adcVal[i]);
        }

        HAL_Delay(15); //增加训练的视觉感受
        setLinght(0, 0, 0); //
        HAL_Delay(15); //增加训练的视觉感受
    }
    zoom = 255.0f / (maxVal - minVal); //计算缩放比例
    MedianVal = (maxVal - minVal) / 2; // 计算中值
    offset = minVal;
#endif
}

void color_Grayscale(uint8_t r, uint8_t g, uint8_t b)
{
#if 0 //rgb -> hsv -> gray
    uint8_t max_Grayscale = 0;
    uint8_t min_Grayscale = 255;
    uint8_t Offset = bgGray;
    uint8_t zoom = 255 / (fgGray>bgGray?fgGray:bgGray);
    int16_t MedianVal = 0;

    rgb2hsv(r, g, b); // 主要为了获取 rgb 值

    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        Grayscale[i] = rgb2gray(Light_RGB[i].r, Light_RGB[i].g, Light_RGB[i].b);
    }
    
    for (uint8_t i = 0; i < SENSORE_NUM; i++) //找最值
    {
        if (Grayscale[i] > max_Grayscale)
            max_Grayscale = Grayscale[i];
        if (Grayscale[i] < min_Grayscale)
            min_Grayscale = Grayscale[i];
    }
   
    if (invert == 0) //无需反转，
    {
        if(min_Grayscale > bgGray) Offset = min_Grayscale; //防止偏移出现负值
        else Offset = bgGray;
        // zoom = 255 / (max_Grayscale - Offset); // 计算缩放比例

        for(uint8_t i = 0; i < SENSORE_NUM; i++)
        {
            Grayscale[i] = (Grayscale[i] - Offset) * zoom;
        }
    }
    else
    {
         if(min_Grayscale > fgGray) Offset = min_Grayscale; //防止偏移出现负值
         else Offset = fgGray;
         
        // zoom = 255 / (max_Grayscale - Offset); // 计算缩放比例

        MedianVal = (max_Grayscale - min_Grayscale) / 2; // 计算中值
        for(uint8_t i = 0; i < SENSORE_NUM; i++) 
        {
            Grayscale[i] +=  (MedianVal - Grayscale[i]) * 2;//围绕中值反转
            Grayscale[i] = (Grayscale[i] - Offset) * zoom; //比例缩放
        }
    }
    for(uint8_t i = 0; i < SENSORE_NUM; i++) // 输出灰度值
    {
        GrayVal[i] = Grayscale[i];
    }

    // printf("%3d %3d %3d %3d %3d %3d\r\n", GrayVal[0], GrayVal[1], GrayVal[2], GrayVal[3], GrayVal[4], GrayVal[5]);
    // printf("in %d \r\n", invert);
#endif
    setLinght(r, g, b);
    HAL_Delay(1);//adc转换时间
    for(uint8_t i = 0; i < SENSORE_NUM; i++) //获取原始值
    {
        Grayscale[i] = VAL4096_2_255(adcVal[i]);   
    }

    for(uint8_t i = 0; i < SENSORE_NUM; i++) 
    {
        Grayscale[i] +=  (MedianVal - Grayscale[i])*2;//围绕中值反转
        Grayscale[i] = (Grayscale[i] - offset) * zoom; //比例缩放
    }
    
    for(uint8_t i = 0; i < SENSORE_NUM; i++) // 输出灰度值
    {
        Grayscale[i] = Grayscale[i]<=255 ? Grayscale[i] : 255;//放大限幅
        Grayscale[i] = Grayscale[i]>=0 ? Grayscale[i] : 0;
        GrayVal[i] = Grayscale[i]; 
    }
}

/**
 * @description: rgb转灰度 （心理学公式）
 * @param {uint8_t} r 红色（0-255）
 * @param {uint8_t} g 绿色（0-255）
 * @param {uint8_t} b 蓝色（0-255）
 * @return {*} 转化后的灰度
 */
static uint8_t rgb2gray(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r * 153 + (uint32_t)g * 300 + (uint32_t)b * 59) >> 9;
}


