#include "myMain.h"

#include "tim.h"
#include "adc.h"
#include "i2c.h"

#include "./inc/sys_config_and_flash.h"
#include "./inc/register.h"
#include "./inc/binary.h"
#include "./inc/gray.h"
#include "./inc/color.h"
#include "./inc/light.h"


typedef void (*key_func_cb)(void);

typedef enum
{
    KEY_IDLE = 0,
    KEY_ON_CLICK,
    KEY_ON_LONG,
} KEY_STATE;

typedef struct
{
    KEY_STATE state;
    key_func_cb on_click;
    key_func_cb on_long;
    uint32_t last_press_time;
    uint8_t press_flag : 1;
    uint8_t tarigger_flag : 1;
} KET_Typedef;



static void key_event_handling(void);
static void other_addr_light_color(void);


static uint8_t iic_read_reg_idle_val[REGISTER_NUM] = {
    [SENSORE_NUM]=MACHINE_IDLE,
};
static uint32_t adcVal[SENSORE_NUM];
static KET_Typedef key_info = {KEY_IDLE};
static MACHINE_STATE machine_state = MACHINE_IDLE;
static const uint16_t other_addr_color[][3] = {
    {1000, 1000, 1000},
    {0, 1000, 1000},
    {1000, 0, 1000},
    {0, 0, 1000},
};



static void key_on_click_cb(void)
{
    machine_state = MACHINE_GRAY_STUDY;
}

static void key_on_long_cb(void)
{
    MAX_I2C_other_addr_changle();
    other_addr_light_color();
}

void setup(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_ADCEx_Calibration_Start(&hadc1);                        // 校准
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcVal, SENSORE_NUM); // 开启adc

    sys_config_info_init(); // 系统配置信息初始化
    other_addr_light_color(); // 显示地址演示灯
    I2C_Slave_Reset(); // 重启从机

    key_info.on_click = key_on_click_cb;
    key_info.on_long = key_on_long_cb;

    Grayscale_Init();
    binary_init();
    color_init();

    iic_read_reg.reg = iic_read_reg_idle_val;
    iic_read_reg.size = 2;
}

void loop(void)
{
    key_event_handling();
    identify_light(machine_state, iic_read_reg.reg);
    if (MACHINE_IDLE == machine_state) // 空闲
    {
        set_rgb_brightness(0, 0, 0);
        iic_read_reg.reg = iic_read_reg_idle_val;
        iic_read_reg.size = REGISTER_NUM;
    }
    else if (MACHINE_GRAY_IDENTIFY == machine_state) // 灰度识别
    {
        grayVal[SENSORE_NUM] = MACHINE_GRAY_IDENTIFY;
        iic_read_reg.reg = grayVal;
        iic_read_reg.size = REGISTER_NUM;
        gary_identify();
    }
    else if (MACHINE_BINARY_IDENTIFY == machine_state) // 二值识别
    {
        binaryVal[SENSORE_NUM] = MACHINE_BINARY_IDENTIFY;
        iic_read_reg.reg = binaryVal;
        iic_read_reg.size = REGISTER_NUM;
        binary_identify();
    }
    else if (MACHINE_COLOR_IDENTIFY == machine_state) // 颜色识别
    {
        colorVal[SENSORE_NUM] = MACHINE_COLOR_IDENTIFY;
        iic_read_reg.reg = colorVal;
        iic_read_reg.size = REGISTER_NUM;
        color_identify();
    }
    else if (MACHINE_GRAY_STUDY == machine_state) // 灰度学习
    {
        grayVal[SENSORE_NUM] = MACHINE_GRAY_STUDY;
        iic_read_reg.reg = grayVal;
        iic_read_reg.size = REGISTER_NUM;
        gary_study();
        machine_state = MACHINE_GRAY_IDENTIFY;
    }
    else if (MACHINE_BINARY_STUDY == machine_state) // 二值学习
    {
        binaryVal[SENSORE_NUM] = MACHINE_BINARY_STUDY;
        iic_read_reg.reg = binaryVal;
        iic_read_reg.size = REGISTER_NUM;
        binary_study();
        machine_state = MACHINE_BINARY_IDENTIFY;
    }
    else if (MACHINE_COLOR_CLEAR_STUDY == machine_state) // 清除颜色学习
    {
        iic_read_reg.reg = iic_read_reg_idle_val;
        iic_read_reg.size = REGISTER_NUM;
        clear_color_study();
        machine_state = MACHINE_IDLE;
    }
    // 颜色学习
    else if (MACHINE_COLOR_RED_STUDY <= machine_state && machine_state <= MACHINE_COLOR_WHITE_STUDY)
    {
        colorVal[SENSORE_NUM] = machine_state;
        iic_read_reg.reg = colorVal;
        iic_read_reg.size = REGISTER_NUM;
        color_study(machine_state - MACHINE_COLOR_RED_STUDY);
        machine_state = MACHINE_COLOR_IDENTIFY;
    }

    // IIC 写入命令处理
    if (1 == iic_write_reg.changle_flag)
    {
        if ( iic_write_reg.reg[0] < MACHINE_STATE_NUM )
        {
            machine_state = (MACHINE_STATE)iic_write_reg.reg[0];
        }
        iic_write_reg.changle_flag = 0;
    }
}

static void other_addr_light_color(void)
{
    uint8_t other_addr_pos = 0;
    const uint16_t *other_addr_color_ptr;
    other_addr_pos = MAX_I2C_addr_get_other();
    other_addr_color_ptr = other_addr_color[other_addr_pos];
    set_address_rgb(other_addr_color_ptr[0], other_addr_color_ptr[1], other_addr_color_ptr[2]);
}

// ======== KEY 状态机 ========
static void key_event_handling(void)
{
    uint8_t key_press = HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin);
    // 按键按下
    if (key_press == 0)
    {
        if (key_info.press_flag == 0)
        {
            key_info.press_flag = 1;
            key_info.last_press_time = HAL_GetTick();
            key_info.state = KEY_IDLE;
            key_info.tarigger_flag = 0;
        }
        else if (key_info.state == KEY_IDLE && (HAL_GetTick() - key_info.last_press_time > 1000))
        {
            key_info.state = KEY_ON_LONG;
            if (!key_info.tarigger_flag)
            {
                key_info.tarigger_flag = 1;
                if (key_info.on_long)
                    key_info.on_long();
            }
        }
    }
    // 按键抬起
    else
    {
        if (key_info.press_flag == 1)
        {
            uint32_t press_duration = HAL_GetTick() - key_info.last_press_time;
            if (press_duration >= 30 && press_duration <= 1000)
            {
                key_info.state = KEY_ON_CLICK;
                if (!key_info.tarigger_flag)
                {
                    key_info.tarigger_flag = 1;
                    if (key_info.on_click)
                        key_info.on_click();
                }
            }
        }
        // 重置状态
        key_info.press_flag = 0;
        key_info.state = KEY_IDLE;
        key_info.tarigger_flag = 0;
    }
}

uint32_t get_adc_val(uint8_t port)
{
    return adcVal[port];
}
