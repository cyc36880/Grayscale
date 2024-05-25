/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-22 16:19:09
 * @LastEditTime: 2024-04-03 16:44:43
 * @FilePath: \MDK-ARMd:\Desktop\iic_Server\myCode\control\control.h
 */
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "main.h"
#include "hsv.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} lv_color_rgb_t;

typedef struct {
    /*
     * 0: 关闭
     * 1: 颜色
     * 2: 灰度
     * 3: 二值
     * 4: 灰度学习
     * 5: 二值学习
    */
    uint8_t sw;
    uint8_t *TXBuf; //发送缓冲区

} Control_t;

// 系统状态标志
enum CONTROL_SW {
    SW_OFF = 0, 
    SW_COLOR,
    SW_GRAY ,
    SW_BINARY ,
    SW_GRAY_LEARN,
    SW_BINARY_LEARN,
};


#define VAL4096_2_255(x) ((uint32_t)x*255/4096)

#define SENSORE_NUM   6   //传感器路数
#define DEFAULT_LIGHT 100 //默认亮度

extern lv_color_hsv_t Light_HSV[6]; //六路hsv
extern lv_color_rgb_t Light_RGB[6]; //六路rgb
extern uint32_t adcVal[6]; //六路adc值
extern Control_t control; //控制句柄

/**
 * @description: 三色灯亮度
 * @param {uint8_t} r 红色 （0-255）
 * @param {uint8_t} g 绿色 （0-255）
 * @param {uint8_t} b 蓝色 （0-255）
 * @return {*}
 */
void setLinght(uint8_t r, uint8_t g, uint8_t b);


void setup(void);
void loop(void);

void control_iic_tx_CpltCallback(void);
void control_iic_rx_CpltCallback(void);

#endif 

