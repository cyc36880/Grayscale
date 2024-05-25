/*
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-01-18 14:23:16
 * @LastEditTime: 2024-01-25 15:26:07
 * @FilePath: \MDK-ARMd:\Desptop\icMatser_RTOS_guider\myCode\hardware\Motor\Motor.h
 */
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "main.h"

#define MOTOR_ADDR  0x50 //通用地址
#define MOTOR_ADDR0 0x51
#define MOTOR_ADDR1 0x52
#define MOTOR_ADDR2 0x53
#define MOTOR_ADDR3 0x54

#define MOTOR_CMD_DISABILITY 0x00 //失能
#define MOTOR_CMD_ROCONSPDD  0x01 //恒速转动（电磁制动）
#define MOTOR_CMD_CONSPDTIMD 0x02 //恒速定时（电磁制动）
#define MOTOR_CMD_CONSPDPOSD 0x03 //恒速定位（电磁制动）
#define MOTOR_CMD_RELDISPD   0x04 //相对位移（电磁制动）
#define MOTOR_CMD_OPLOPTURG  0x05 //开环转动
#define MOTOR_CMD_ROCONSPDG  0x11 //恒速转动（惯性制动）
#define MOTOR_CMD_CONSPDTIMG 0x12 //恒速定时（惯性制动）
#define MOTOR_CMD_CONSPDPOSG 0x13 //恒速定位（惯性制动）
#define MOTOR_CMD_RELDISPG   0x14 //相对位移（惯性制动）
#define MOTOR_CMD_SETCURLOC  0x70 //设置当前位置
#define MOTOR_CMD_CUSTID     0x79 //自定义ID

#define MOTOR_STA_DISABILITY 0x00 //失能
#define MOTOR_STA_ROCONSPDD  0x01 //恒速转动（电磁制动）
#define MOTOR_STA_CONSPDTIMD 0x02 //恒速定时（电磁制动）
#define MOTOR_STA_CONSPDPOSD 0x03 //恒速定位（电磁制动）
#define MOTOR_STA_RELDISPD   0x04 //相对位移（电磁制动）
#define MOTOR_STA_OPLOPTURG  0x05 //开环转动
#define MOTOR_STA_ROCONSPDG  0x06 //恒速转动（惯性制动）
#define MOTOR_STA_CONSPDTIMG 0x07 //恒速定时（惯性制动）
#define MOTOR_STA_CONSPDPOSG 0x08 //恒速定位（惯性制动）
#define MOTOR_STA_RELDISPG   0x09 //相对位移（惯性制动）
#define MOTOR_STA_SETCURLOC  0x10 //堵转停止
#define MOTOR_STA_CUSTID     0x11 //运动结束停止

#pragma anon_unions
typedef struct MOTORCMD
{
  uint8_t id; //id
  uint8_t cmd; //指令
  uint8_t speed; //速度
  union {
    uint16_t Time;     //时间 高8+低8
    int16_t Location; //位置
  };
}TypedefMotorCmd;

typedef struct MOTORDATA
{
  int8_t speed;
  uint8_t sta;
  int16_t location;
  uint16_t power;
}TypedefMotorDat;


typedef struct MOTOSTA
{
  uint8_t isOnline; //是否在线
  TypedefMotorCmd motor_cmd;
  TypedefMotorDat motor_dat; //电机
}TypedefMotorSta;


extern TypedefMotorSta Motor_0;

HAL_StatusTypeDef MotorSendCmd(uint8_t id, uint8_t cmd, uint8_t speed, int16_t TimeOrLocation);
HAL_StatusTypeDef MotorSendCmdpack(TypedefMotorCmd *pack);
HAL_StatusTypeDef MotorGetData(uint8_t id, TypedefMotorDat *data);


#endif


