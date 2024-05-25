/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 09:42:56
 * @LastEditTime: 2024-03-23 16:51:36
 * @FilePath: \MDK-ARMd:\Desktop\myIIC\iic_Server\iic_Server\myCode\control\hsv.c
 */
#include "hsv.h"

#include "control.h"

// 色调(H)，饱和度(S)，明度(V)
lv_color_hsv_t lv_color_rgb_to_hsv(uint8_t r8, uint8_t g8, uint8_t b8)
{
    uint16_t r = ((uint32_t)r8 << 10) / 255;
    uint16_t g = ((uint32_t)g8 << 10) / 255;
    uint16_t b = ((uint32_t)b8 << 10) / 255;

    uint16_t rgbMin = r < g ? (r < b ? r : b) : (g < b ? g : b);
    uint16_t rgbMax = r > g ? (r > b ? r : b) : (g > b ? g : b);

    lv_color_hsv_t hsv;

    // https://en.wikipedia.org/wiki/HSL_and_HSV#Lightness
    hsv.v = (100 * rgbMax) >> 10;

    int32_t delta = rgbMax - rgbMin;
    if(delta < 3) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    // https://en.wikipedia.org/wiki/HSL_and_HSV#Saturation
    hsv.s = 100 * delta / rgbMax;
    if(hsv.s < 3) {
        hsv.h = 0;
        return hsv;
    }

    // https://en.wikipedia.org/wiki/HSL_and_HSV#Hue_and_chroma
    int32_t h;
    if(rgbMax == r)
        h = (((g - b) << 10) / delta) + (g < b ? (6 << 10) : 0); // between yellow & magenta
    else if(rgbMax == g)
        h = (((b - r) << 10) / delta) + (2 << 10); // between cyan & yellow
    else if(rgbMax == b)
        h = (((r - g) << 10) / delta) + (4 << 10); // between magenta & cyan
    else
        h = 0;
    h *= 60;
    h >>= 10;
    if(h < 0) h += 360;

    hsv.h = h;
    return hsv;
}

/**
 * @description: r g b 转 hsv
 * @param {uint8_t} r 红灯亮度（0-255）
 * @param {uint8_t} g 绿灯亮度（0-255）
 * @param {uint8_t} b 蓝灯亮度（0-255）
 * @return {*}
 */
void rgb2hsv(uint8_t r, uint8_t g, uint8_t b)
{
    setLinght(r, 0, 0);
	HAL_Delay(1);
	for(int i=0;i<6;i++)
	{
		Light_RGB[i].r = VAL4096_2_255(adcVal[i]);
	}
    setLinght(0, g, 0);
	HAL_Delay(1);
	for(int i=0;i<6;i++)
	{
		Light_RGB[i].g = VAL4096_2_255(adcVal[i]);
	}
    setLinght(0, 0, b);
	HAL_Delay(1);
	for(int i=0;i<6;i++)
	{
		Light_RGB[i].b = VAL4096_2_255(adcVal[i]);
	}
    setLinght(0, 0, 0);
    
	for(int i=0;i<6;i++)
	{
	    Light_HSV[i] = lv_color_rgb_to_hsv(Light_RGB[i].r, Light_RGB[i].g, Light_RGB[i].b);
	}
}

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
void lv_color_hsv_to_rgb(uint16_t h, uint8_t s, uint8_t v, uint8_t *cr, uint8_t *cg, uint8_t *cb)
{
    h = (uint32_t)((uint32_t)h * 255) / 360;
    s = (uint16_t)((uint16_t)s * 255) / 100;
    v = (uint16_t)((uint16_t)v * 255) / 100;

    uint8_t r, g, b;

    uint8_t region, remainder, p, q, t;

    if(s == 0) {
        *cr = v;
        *cg = v;
        *cb = v;
        return ;
    }

    region    = h / 43;
    remainder = (h - (region * 43)) * 6;

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch(region) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        default:
            r = v;
            g = p;
            b = q;
            break;
    }
    
    *cr = r;
    *cg = g;
    *cb = b;
}
