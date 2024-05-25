#include "LED_RGB.h"



void LED_RBG_SET(uint8_t val)
{
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, val&(1<<0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, val&(1<<1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, val&(1<<2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

