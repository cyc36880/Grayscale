/*
 * @LastEditors: 蔡雅超
 * @Date: 2024-04-03 16:48:13
 * @LastEditTime: 2024-04-03 17:06:43
 * @FilePath: \MDK-ARMd:\Desktop\iic_Server\myCode\control\flash.c
 */
#include "flash.h"

#define FLASH_BUF_LEN  100

static uint16_t flash_buf[FLASH_BUF_LEN] = {0};


static void updata_flash_data(uint16_t *data, uint16_t len);

/**
 * 
 * uint16_t flash buf[] = {...}
 * 
 * [0]    从机地址
 * [1-10]  灰度
 * [6-20] 二值数据
 * 
 * 
*/



/*************************************************************
 * @description: 读取flash数据
 *  @note 从flash中读取一些数据
 * @return {*}
 *************************************************************/
void flash_Init(void)
{
    for(uint8_t i=0; i<FLASH_BUF_LEN; i++)
    {
        flash_buf[i] = ((uint16_t*)MemoryAddr)[i];
    }
}

void updata_flash(void)
{
    updata_flash_data(flash_buf, FLASH_BUF_LEN);
}

static void updata_flash_data(uint16_t *data, uint16_t len)
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

    for(uint8_t i=0; i<len; i++)
    {
         HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, MemoryAddr+i*2, data[i]);
    }
    //4、锁住FLASH
    HAL_FLASH_Lock();
}

uint16_t *get_flash_buf(void)
{
    return flash_buf;
}



// void Write_Flash_Data(uint16_t data)
// {
//      //1、解锁FLASH
//     HAL_FLASH_Unlock();

//     //2、擦除FLASH
//     FLASH_EraseInitTypeDef f;
//     f.TypeErase = FLASH_TYPEERASE_PAGES;
//     f.Banks = MemoryBank;    //选择存储地址所在的扇区
//     f.PageAddress = MemoryAddr;//选择存储地址所在的页
//     f.NbPages = 1;

//     //设置PageError
//     uint32_t PageError = 0;
//     //调用擦除函数
//     HAL_FLASHEx_Erase(&f, &PageError);
//     HAL_Delay(5);
//     //3、对FLASH烧写
//     HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, MemoryAddr, data);
//     //4、锁住FLASH
//     HAL_FLASH_Lock();
// }




