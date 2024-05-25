/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-04-03 16:48:13
 * @LastEditTime: 2024-04-03 17:07:32
 * @FilePath: \MDK-ARMd:\Desktop\iic_Server\myCode\control\flash.h
 */
#ifndef _FLASH_H_
#define _FLASH_H_

#include "main.h"

#define MemoryAddr 0x800FC00
#define MemoryBank FLASH_BANK_1
#define MemoryPage 63


uint16_t Get_Flash_Data(void);
void Write_Flash_Data(uint16_t data);

#endif

