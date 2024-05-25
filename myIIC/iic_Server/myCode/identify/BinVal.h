/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-24 13:49:07
 * @LastEditTime: 2024-03-27 10:39:00
 * @FilePath: \MDK-ARMd:\Desktop\灰度\myIIC\iic_Server\iic_Server\myCode\identify\BinVal.h
 */
#ifndef _BINVAL_H_
#define _BINVAL_H_

#include "control.h"


typedef struct
{
    uint8_t studyFlag;
    uint8_t Light;
}BinVal_t;


extern BinVal_t BinValControl; //二值识别控制句柄
extern uint8_t bincolor[SENSORE_NUM];

void BinVal_Init(void);
void study_BinVal(uint8_t light);
void color_Bin(uint8_t light);



#endif
