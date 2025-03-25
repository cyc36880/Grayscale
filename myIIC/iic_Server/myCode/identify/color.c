/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 10:31:50
 * @LastEditTime: 2024-03-31 14:26:42
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\identify\color.c
 */
#include "color.h"

#include "hsv.h"
#include "stdlib.h"
#include "control.h"
#include "flash.h"

#define SORT_SIZE 10

#define H_EXP 1
#define S_EXP 0
#define V_EXP 0

static void update_Grayscale_flashdata(void);

#define COLOR_BUF_ADDR_OFFSET  200

static lv_color_hsv_t color_table[] = {
    // 色表
    HSV_RED,
    // HSV_NONE,

    HSV_YELLOW,
    // HSV_NONE,

    HSV_GREEN,
    // HSV_NONE,

    HSV_CYAN,
    // HSV_NONE,

    HSV_BLUE,
    // HSV_NONE,

    HSV_PURPLE,
    // HSV_NONE,
};

static lv_color_hsv_t color_buf[sizeof(color_table) / sizeof(color_table[0])][SENSORE_NUM] = {0};

static lv_color_hsv_t sort_buf[SORT_SIZE] = {0};
static lv_color_hsv_t median[SORT_SIZE] = {0};

// static int16_t color_exp[sizeof(color_table)/sizeof(color_table[0])] = {0};

struct COLORBIAS
{
    int16_t bias[sizeof(color_table) / sizeof(color_table[0])];
};

color_t colorControl = // 颜色控制句柄
{
    .rLight = 200,
    .gLight = 200,
    .bLight = 200,
};

// 颜色对比偏差
static struct COLORBIAS color_bias[sizeof(color_table) / sizeof(color_table[0])] = {0};

uint8_t color[SENSORE_NUM] = {0}; // 识别后的颜色索引

static void color_hsv_sort_h(lv_color_hsv_t *hsv_buf, uint8_t len)
{
    lv_color_hsv_t hsv;
    for (uint8_t i = 0; i < len - 1; i++)
    {
        for (uint8_t j = i + 1; j < len; j++)
        {
            if (hsv_buf[j].h < hsv_buf[i].h)
            {
                hsv = hsv_buf[i];
                hsv_buf[i] = hsv_buf[j];
                hsv_buf[j] = hsv;
            }
        }
    }
}

static uint16_t getColorOffset(uint16_t max, uint16_t thr, uint16_t cVal)
{
    int16_t val = (thr + cVal) / 2;
    uint16_t val1 = abs(val - thr);
    uint16_t val2 = abs(val - cVal);

    if ((val1 + val2) < max / 2)
        return val1 + val2;
    else
        return max - (val1 + val2);
}

void color_init(void)
{
    uint8_t *bufStart = (uint8_t *) ((uint8_t*)get_flash_buf() + COLOR_BUF_ADDR_OFFSET);
    for(uint8_t i=0; i<SENSORE_NUM; i++)
    {
        for (uint8_t j=0; j<sizeof(color_table) / sizeof(color_table[0]); j++)
        {
            bufStart += 1;
            color_buf[i][j].h = *bufStart;
            printf("color_buf[%d][%d].h = %d\n", i, j, color_buf[i][j].h);
        }
        printf("\n");
    }

    // for (uint8_t i = 0; i < SENSORE_NUM; i++)
    // {
    //     for (uint8_t j = 0; j < sizeof(color_table) / sizeof(color_table[0]); j++)
    //     {
    //         color_buf[i][j].h = color_table[j].h;
    //         color_buf[i][j].s = color_table[j].s;
    //         color_buf[i][j].v = color_table[j].v;
    //     }
    // }
}

void color_study(uint8_t w_color)
{
    if (w_color >= sizeof(color_table) / sizeof(color_table[0]))
        return;

    for (uint8_t port = 0; port < SENSORE_NUM; port++)
    {
        for (uint8_t i = 0; i < SORT_SIZE; i++) // 取十次排序后的中值，并排序
        {
            for (uint8_t j = 0; j < SORT_SIZE; j++) // 取十次数，并排序
            {
                rgb2hsv(colorControl.rLight, colorControl.gLight, colorControl.bLight);
                sort_buf[j].h = Light_HSV[port].h;
                sort_buf[j].s = Light_HSV[port].s;
                sort_buf[j].v = Light_HSV[port].v;
            }
            setLinght(255, 255, 255); //
            HAL_Delay(15);            // 增加训练的视觉感受
            setLinght(0, 0, 0);       //
            HAL_Delay(15);            // 增加训练的视觉感受
            color_hsv_sort_h(sort_buf, SORT_SIZE);
            median[i].h = sort_buf[SORT_SIZE / 2].h;
            median[i].s = sort_buf[SORT_SIZE / 2].s;
            median[i].v = sort_buf[SORT_SIZE / 2].v;
        }
        color_hsv_sort_h(median, SORT_SIZE);
        color_buf[port][w_color].h = median[SORT_SIZE / 2].h;
        color_buf[port][w_color].s = median[SORT_SIZE / 2].s;
        color_buf[port][w_color].v = median[SORT_SIZE / 2].v;
    }
    update_Grayscale_flashdata(); // 更新颜色数据到flash
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

    for (int8_t i = 0; i < SENSORE_NUM; i++)
    {
        for (uint8_t j = 0; j < sizeof(color_table) / sizeof(color_table[0]); j++)
        {
            // color_bias[i].bias[j] = getColorOffset(360, color_table[j].h, Light_HSV[i].h);
            color_bias[i].bias[j] = getColorOffset(360, color_buf[i][j].h, Light_HSV[i].h);
        }
    }
    // 找出偏差最小的颜色
    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        if (Light_HSV[i].s < 15 && Light_HSV[i].v < 15) // 亮度太低，直接跳过
        {
            color[i] = 0;
            continue;
        }

        // 找出偏差最小的颜色
        int8_t min_index = 0;

        for (uint8_t j = 1; j < sizeof(color_buf) / sizeof(color_buf[0]); j++)
        {
            if (color_bias[i].bias[j] < color_bias[i].bias[min_index])
            {
                min_index = j;
            }
        }
        color[i] = min_index + 1;
        // if(color_bias[i].bias[min_index] > 30) color[i] = 0; // 偏差过大，判定为无颜色
    }
}


static void update_Grayscale_flashdata(void)
{
    uint8_t *bufStart = (uint8_t *) ((uint8_t *)get_flash_buf() + COLOR_BUF_ADDR_OFFSET);

    for(uint8_t i=0; i<SENSORE_NUM; i++)
    {
        for (uint8_t j=0; j<sizeof(color_table) / sizeof(color_table[0]); j++)
        {
            bufStart += 1;
            *bufStart = color_buf[i][j].h;
        }
    }
    updata_flash();
}



void clear_color_study(void)
{
    uint8_t *bufStart = (uint8_t *) ((uint8_t *)get_flash_buf() + COLOR_BUF_ADDR_OFFSET);

    for(uint8_t i=0; i<SENSORE_NUM; i++)
    {
        for (uint8_t j=0; j<sizeof(color_table) / sizeof(color_table[0]); j++)
        {
            bufStart += 1;
            color_buf[i][j].h = 0;
            *bufStart = color_buf[i][j].h;
        }
    }
    updata_flash();
}

