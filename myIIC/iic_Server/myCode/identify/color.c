/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 10:31:50
 * @LastEditTime: 2024-03-31 14:26:42
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\identify\color.c
 */
#include "color.h"

#include "hsv.h"
#include "stdlib.h"

#define H_EXP 1
#define S_EXP 0
#define V_EXP 0

const lv_color_hsv_t color_table[]= { //色表
    HSV_RED,
    //HSV_ORANGE,
    HSV_YELLOW,
    HSV_GREEN,
    HSV_CYAN,
    HSV_BLUE,
    HSV_PURPLE,
};


//static int16_t color_exp[sizeof(color_table)/sizeof(color_table[0])] = {0};


struct COLORBIAS
{
    int16_t bias[sizeof(color_table)/sizeof(color_table[0])];
};

color_t colorControl =  // 颜色控制句柄
{
    .rLight = 200,
    .gLight = 200,
    .bLight = 200,
};

// 颜色对比偏差
static struct COLORBIAS color_bias[sizeof(color_table)/sizeof(color_table[0])] = {0};

uint8_t color[SENSORE_NUM] = {0}; //识别后的颜色索引



static uint16_t getColorOffset(uint16_t max, uint16_t thr, uint16_t cVal)
{
    int16_t val = (thr + cVal)/2;
    uint16_t val1 = abs(val-thr);
    uint16_t val2 = abs(val-cVal);

    if( (val1 + val2) < max/2) return val1 + val2;
    else return max - (val1 + val2);
}

void color_init(void)
{
//    //按 比例 得 期望
//    for(uint8_t i=0; i<sizeof(color_table)/sizeof(color_table[0]); i++)
//    {
//        color_exp[i] = color_table[i].h*H_EXP + color_table[i].s*S_EXP + color_table[i].v*V_EXP;
//    }
    // printf("%d %d %d %d %d %d %d\n", color_exp[0], color_exp[1], color_exp[2], color_exp[3], color_exp[4], color_exp[5], color_exp[6]);
}


/**
 * @description: 颜色识别
 * @param {uint8_t} r 红灯亮度（0-255）
 * @param {uint8_t} g 绿灯亮度（0-255）
 * @param {uint8_t} b 蓝灯亮度（0-255）
 * @return {*}
 */
void Color_Recongnition(uint8_t r, uint8_t g, uint8_t b)
{
    rgb2hsv(r, g, b);

    for(int8_t i=0; i<SENSORE_NUM; i++)
    {
        for(uint8_t j=0; j<sizeof(color_table)/sizeof(color_table[0]); j++)
        {
            // color_bias[i].bias[j] = abs( getColorOffset(360, color_table[j].h, Light_HSV[i].h)*H_EXP + getColorOffset(100, color_table[j].s, Light_HSV[i].s)*S_EXP + getColorOffset(100, color_table[j].v, Light_HSV[i].v)*V_EXP - color_exp[j] );
            // printf("%d ", color_bias[i].bias[j]);
            color_bias[i].bias[j]  = getColorOffset(360, color_table[j].h, Light_HSV[i].h);  
            // color_bias[i].bias[j] += getColorOffset(100, color_table[j].s, Light_HSV[i].s);
            // color_bias[i].bias[j] += getColorOffset(100, color_table[j].v, Light_HSV[i].v);
        }
        // printf("\r\n");
    }
    // 找出偏差最小的颜色
    for(uint8_t i=0; i<SENSORE_NUM; i++)
    {
        if(Light_HSV[i].s<15 && Light_HSV[i].v<15) // 亮度太低，直接跳过
        {
            color[i] = 0;
            continue;
        }
        
        // 找出偏差最小的颜色
        int8_t min_index = 0;

        for(uint8_t j=1; j<sizeof(color_table)/sizeof(color_table[0]); j++)
        {
            if(color_bias[i].bias[j] < color_bias[i].bias[min_index])
            {
                min_index = j;
            }
        }
        color[i] = min_index+1;
        // if(color_bias[i].bias[min_index] > 30) color[i] = 0; // 偏差过大，判定为无颜色
    }
}



