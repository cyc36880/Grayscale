/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 09:42:56
 * @LastEditTime: 2024-03-23 16:12:48
 * @FilePath: \MDK-ARMd:\Desktop\myIIC\iic_Server\iic_Server\myCode\control\hsv.h
 */
#ifndef _HSV_H_
#define _HSV_H_

#include "main.h"

typedef struct {
    /* @param h hue [0..359]
    * @param s saturation [0..100]
    * @param v value [0..100]
    */
    uint16_t h;
    uint8_t s;
    uint8_t v;
} lv_color_hsv_t;

// 色调(H)，饱和度(S)，明度(V)
lv_color_hsv_t lv_color_rgb_to_hsv(uint8_t r8, uint8_t g8, uint8_t b8);

/**
 * @description: r g b 转 hsv
 * @param {uint8_t} r 红灯亮度（0-255）
 * @param {uint8_t} g 绿灯亮度（0-255）
 * @param {uint8_t} b 蓝灯亮度（0-255）
 * @return {*}
 */
void rgb2hsv(uint8_t r, uint8_t g, uint8_t b);

/**
 * @description:  hsv 转 rgb
 * @param {uint16_t} h 色调（0-360）
 * @param {uint8_t} s  饱和度（0-100）
 * @param {uint8_t} v  明度（0-100）
 * @param {uint8_t} *cr 红色指针
 * @param {uint8_t} *cg 绿色指针
 * @param {uint8_t} *cb 蓝色指针
 * @return {*}
 */
void lv_color_hsv_to_rgb(uint16_t h, uint8_t s, uint8_t v, uint8_t *cr, uint8_t *cg, uint8_t *cb);

#endif



