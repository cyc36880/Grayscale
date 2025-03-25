/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 10:31:50
 * @LastEditTime: 2024-03-31 14:06:51
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\identify\color.h
 */
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

#include "control.h"

// 积木
// #define HSV_RED       {0, 46, 3}   //红
// #define HSV_ORANGE    {7, 61, 5} //橙
// #define HSV_YELLOW    {68, 50, 5}   //黄
// #define HSV_GREEN     {192, 71, 2}   //绿
// #define HSV_CYAN      {183, 77, 8} //青
#define HSV_BLUE      {220, 75, 6}  //蓝
// #define HSV_PURPLE    {330, 18, 3}  //紫

// 色卡
#define HSV_RED       {0, 57, 5}   //红   1
// #define HSV_ORANGE    {20, 52, 6} //橙
#define HSV_YELLOW    {88, 60, 10}   //黄 2
#define HSV_GREEN     {135, 70, 7}   //绿 3
#define HSV_CYAN      {190, 75, 10}  //青 4
// #define HSV_BLUE      {210, 90, 10}  //蓝 5
#define HSV_PURPLE    {222, 69, 5}  //紫 6
#define HSV_WHITE     {198, 44, 13}    //白
#define HSV_BLACK     {0, 0, 0}     //黑

#define HSV_NONE      {0, 0, 0}     //无

typedef struct{
    uint8_t rLight;
    uint8_t gLight;
    uint8_t bLight;
    uint8_t studyFlag;
 }color_t;


extern color_t colorControl; //颜色控制句柄

extern lv_color_hsv_t color_table[]; //颜色表

/**
 * @description: 颜色识别
 * @param {uint8_t} r 红灯亮度（0-255）
 * @param {uint8_t} g 绿灯亮度（0-255）
 * @param {uint8_t} b 蓝灯亮度（0-255）
 * @return {*} 
 */
void Color_Recongnition(uint8_t r, uint8_t g, uint8_t b);
extern uint8_t color[SENSORE_NUM]; //识别后的颜色索引
void color_study(uint8_t w_color);
void clear_color_study(void);
void color_init(void);

#endif
