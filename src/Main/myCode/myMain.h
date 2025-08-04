/*
 * @Author       : 蔡雅超 (zishen)
 * @LastEditors  : zishen
 * @Date         : 2025-07-31 21:42:56
 * @LastEditTime : 2025-08-04 16:52:44
 * @Description  : 
 * Copyright (c) 2025 Author 蔡雅超 email: 2672632650@qq.com, All Rights Reserved.
 */
#ifndef __MYMAIN_H__
#define __MYMAIN_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "string.h"

#define SENSORE_NUM    6                 //传感器路数
#define REGISTER_NUM   (SENSORE_NUM+1)   //状态寄存器，位于最后一位

#define IDENTIFY_THRESHOLD (170) //灰度识别阈值
#define SERVER_ADDRESS     0x6F  //从机主地址

typedef enum
{
    MACHINE_IDLE = 0,
    MACHINE_COLOR_IDENTIFY,
    MACHINE_GRAY_IDENTIFY,
    MACHINE_BINARY_IDENTIFY,

    MACHINE_GRAY_STUDY,
    MACHINE_BINARY_STUDY,
    MACHINE_COLOR_CLEAR_STUDY,
    MACHINE_COLOR_RED_STUDY,
    MACHINE_COLOR_GREEN_STUDY,
    MACHINE_COLOR_BLUE_STUDY,
    MACHINE_COLOR_YELLOW_STUDY,
    MACHINE_COLOR_CYAN_STUDY,
    MACHINE_COLOR_PUPRLE_STUDY,
    MACHINE_COLOR_BLACK_STUDY,
    MACHINE_COLOR_WHITE_STUDY,

    MACHINE_STATE_NUM,
} MACHINE_STATE;


void setup(void);
void loop(void);

/**
 * @description: 得到通道的adc值
 * @param {uint8_t} port 0~(SENSORE_NUM-1)
 * @return {*}
 */
uint32_t get_adc_val(uint8_t port);

#ifdef __cplusplus
}
#endif


#endif /* __MYMAIN_H__ */

