/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-28 16:12:58
 * @LastEditTime: 2024-03-29 13:35:15
 * @FilePath: \MDK-ARMd:\Desktop\灰度\pro\myIIC\iic_Server\iic_Server\myCode\hardware\LED\LED.h
 */
#ifndef _LED_H_
#define _LED_H_

#include "main.h"

void LED_Init(void);
void LED_Run(void);


void LED_SET_ALL(uint8_t sta);

#endif

