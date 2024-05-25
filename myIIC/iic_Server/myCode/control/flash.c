/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-04-03 16:48:13
 * @LastEditTime: 2024-04-03 17:06:43
 * @FilePath: \MDK-ARMd:\Desktop\iic_Server\myCode\control\flash.c
 */
#include "flash.h"



uint16_t Get_Flash_Data(void)
{
    return *(uint16_t*)MemoryAddr;
}

void Write_Flash_Data(uint16_t data)
{
     //1、解锁FLASH
    HAL_FLASH_Unlock();

    //2、擦除FLASH
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.Banks = MemoryBank;    //选择存储地址所在的扇区
    f.PageAddress = MemoryAddr;//选择存储地址所在的页
    f.NbPages = 1;

    //设置PageError
    uint32_t PageError = 0;
    //调用擦除函数
    HAL_FLASHEx_Erase(&f, &PageError);
    HAL_Delay(5);
    //3、对FLASH烧写
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, MemoryAddr, data);
    //4、锁住FLASH
    HAL_FLASH_Lock();
}




