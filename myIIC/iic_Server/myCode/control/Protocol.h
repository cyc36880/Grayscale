/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-03-23 09:33:03
 * @LastEditTime: 2024-04-03 17:21:47
 * @FilePath: \MDK-ARMd:\Desktop\iic_Server\myCode\control\Protocol.h
 */
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "main.h"

#define SERVER_ADDRESS    0x70 //从机主地址

extern const uint8_t SecAlaveAddrArray[4];
extern uint8_t SecAlaveAddrPos; 


void ServerI2C_Init(void);

/**
 * @description: iic意外，重新初始化iic
 * @return {*}
 */
void I2C_Slave_Rest(void); //初始化I2C从机接收态

#endif


