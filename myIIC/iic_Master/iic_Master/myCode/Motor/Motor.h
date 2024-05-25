/*
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-01-18 14:23:16
 * @LastEditTime: 2024-01-25 15:26:07
 * @FilePath: \MDK-ARMd:\Desptop\icMatser_RTOS_guider\myCode\hardware\Motor\Motor.h
 */
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "main.h"

#define MOTOR_ADDR  0x50 //ͨ�õ�ַ
#define MOTOR_ADDR0 0x51
#define MOTOR_ADDR1 0x52
#define MOTOR_ADDR2 0x53
#define MOTOR_ADDR3 0x54

#define MOTOR_CMD_DISABILITY 0x00 //ʧ��
#define MOTOR_CMD_ROCONSPDD  0x01 //����ת��������ƶ���
#define MOTOR_CMD_CONSPDTIMD 0x02 //���ٶ�ʱ������ƶ���
#define MOTOR_CMD_CONSPDPOSD 0x03 //���ٶ�λ������ƶ���
#define MOTOR_CMD_RELDISPD   0x04 //���λ�ƣ�����ƶ���
#define MOTOR_CMD_OPLOPTURG  0x05 //����ת��
#define MOTOR_CMD_ROCONSPDG  0x11 //����ת���������ƶ���
#define MOTOR_CMD_CONSPDTIMG 0x12 //���ٶ�ʱ�������ƶ���
#define MOTOR_CMD_CONSPDPOSG 0x13 //���ٶ�λ�������ƶ���
#define MOTOR_CMD_RELDISPG   0x14 //���λ�ƣ������ƶ���
#define MOTOR_CMD_SETCURLOC  0x70 //���õ�ǰλ��
#define MOTOR_CMD_CUSTID     0x79 //�Զ���ID

#define MOTOR_STA_DISABILITY 0x00 //ʧ��
#define MOTOR_STA_ROCONSPDD  0x01 //����ת��������ƶ���
#define MOTOR_STA_CONSPDTIMD 0x02 //���ٶ�ʱ������ƶ���
#define MOTOR_STA_CONSPDPOSD 0x03 //���ٶ�λ������ƶ���
#define MOTOR_STA_RELDISPD   0x04 //���λ�ƣ�����ƶ���
#define MOTOR_STA_OPLOPTURG  0x05 //����ת��
#define MOTOR_STA_ROCONSPDG  0x06 //����ת���������ƶ���
#define MOTOR_STA_CONSPDTIMG 0x07 //���ٶ�ʱ�������ƶ���
#define MOTOR_STA_CONSPDPOSG 0x08 //���ٶ�λ�������ƶ���
#define MOTOR_STA_RELDISPG   0x09 //���λ�ƣ������ƶ���
#define MOTOR_STA_SETCURLOC  0x10 //��תֹͣ
#define MOTOR_STA_CUSTID     0x11 //�˶�����ֹͣ

#pragma anon_unions
typedef struct MOTORCMD
{
  uint8_t id; //id
  uint8_t cmd; //ָ��
  uint8_t speed; //�ٶ�
  union {
    uint16_t Time;     //ʱ�� ��8+��8
    int16_t Location; //λ��
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
  uint8_t isOnline; //�Ƿ�����
  TypedefMotorCmd motor_cmd;
  TypedefMotorDat motor_dat; //���
}TypedefMotorSta;


extern TypedefMotorSta Motor_0;

HAL_StatusTypeDef MotorSendCmd(uint8_t id, uint8_t cmd, uint8_t speed, int16_t TimeOrLocation);
HAL_StatusTypeDef MotorSendCmdpack(TypedefMotorCmd *pack);
HAL_StatusTypeDef MotorGetData(uint8_t id, TypedefMotorDat *data);


#endif


