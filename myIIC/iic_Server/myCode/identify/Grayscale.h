/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 15:38:09
 * @LastEditTime: 2024-03-29 10:55:39
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\identify\Grayscale.h
 */
#ifndef _GRAYSCALE_H_
#define _GRAYSCALE_H_

#include "main.h"
#include "control.h"

typedef struct
{
    uint8_t studyFlag;
    uint8_t rLight;
    uint8_t gLight;
    uint8_t bLight;
}Grayscale_t;


typedef struct 
{
   int32_t maxVal;
   int32_t minVal;
   int32_t MedianVal;
   float zoom;
   int32_t offset;
}Grayscale_Port;


extern Grayscale_t GrayControl; //灰度控制句柄
extern uint8_t GrayVal[SENSORE_NUM];

extern Grayscale_Port Grayscale_Port_param[6];

void Grayscale_Init(void);
void study_Grayscale(uint8_t r, uint8_t g, uint8_t b);
void color_Grayscale(uint8_t r, uint8_t g, uint8_t b);






#endif


