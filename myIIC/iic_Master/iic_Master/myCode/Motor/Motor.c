/*
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-01-18 14:23:16
 * @LastEditTime: 2024-01-26 11:13:38
 * @FilePath: \MDK-ARMd:\Desptop\icMatser_RTOS_guider_3\myCode\hardware\Motor\Motor.c
 */

#include "Motor.h"

// #include "i2c.h"
#include "sys.h"
#include "myiic.h"

TypedefMotorSta Motor_0 = 
{
  .isOnline = 0,
  .motor_cmd.id=MOTOR_ADDR,
  .motor_cmd.cmd = MOTOR_CMD_CONSPDTIMD,
  .motor_cmd.speed = 20,
  .motor_cmd.Time = 100,
};


// static void hi2cRest(void)
// {
  // HAL_I2C_DeInit(&hi2c2);        //释放IO口为GPIO，复位句柄状态标志
  // PBout(10) = 1;
  // PBout(11) = 1;
  // HAL_I2C_Init(&hi2c2);
// }

/**
 * @description: 电机命令发送
 * @param {uint8_t} id  电机id
 * @param {uint8_t} cmd 命令
 * @param {uint8_t} speed 速度
 * @param {uint16_t} TimeOrLocation 时间或位置
 * @return {HAL_StatusTypeDef} 
 */
HAL_StatusTypeDef MotorSendCmd(uint8_t id, uint8_t cmd, uint8_t speed, int16_t TimeOrLocation)
{
  // uint8_t sta;
  // uint8_t sendbuf[] = {cmd, speed, TimeOrLocation>>8, TimeOrLocation&0xff};
  // sta = HAL_I2C_Master_Transmit(&hi2c2, (id<<1)|0, sendbuf, sizeof(sendbuf), 0xffff);

  // if(sta == HAL_BUSY) hi2cRest();

  // return sta;
  for( ; ; )
  {
    I2C_Start();

    I2C_SendByte((id<<1)|IIC_W);
    if(I2C_RecvACK()) break;

    I2C_SendByte(cmd);
    if(I2C_RecvACK()) break;

    I2C_SendByte(speed);
    if(I2C_RecvACK()) break;

    I2C_SendByte((uint16_t)TimeOrLocation>>8);
    if(I2C_RecvACK()) break;

    I2C_SendByte((uint16_t)TimeOrLocation&0xff);
    if(I2C_RecvACK()) break;

    I2C_Stop();
    return 0;
  }
  return 1;
}


/**
 * @description: 发送命令包
 * @param {TypedefMotorCmd} *pack 包结构体
 * @return {uint8_t}  0成功 1失败
 */
HAL_StatusTypeDef MotorSendCmdpack(TypedefMotorCmd *pack)
{
  return MotorSendCmd(pack->id, pack->cmd, pack->speed, pack->Time);
}


/**
 * @description: 读取电机信息
 * @param {uint8_t} id 电机id
 * @param {TypedefMotorDat} *data 读取数据结构
 * @return {HAL_StatusTypeDef} 
 */
HAL_StatusTypeDef MotorGetData(uint8_t id, TypedefMotorDat *data)
{
  // uint8_t sta;
  // uint8_t rxbuf[6];
  // sta = HAL_I2C_Master_Receive(&hi2c2, (id<<1)|1, rxbuf, 6, 0xffff);
  // if(sta == HAL_OK)
  // {
  //   data->speed = rxbuf[0];
  //   data->location = ((uint16_t)rxbuf[1]<<8) | rxbuf[2];
  //   data->power = ((uint16_t)rxbuf[3]<<8) | rxbuf[4];
  //   data->sta = rxbuf[5];
  // }
  // else if(sta == HAL_BUSY) 
  //   hi2cRest();
    
  // return sta;
  int8_t speed;
  for( ; ; )
  {
    I2C_Start();

    I2C_SendByte((id<<1)|IIC_R);
    if(I2C_RecvACK()) break;

    speed = I2C_RecvByte(1);
//    if(speed < 0) speed = -speed;
    data->speed = speed;

    data->location = I2C_RecvByte(1);
    data->location <<= 8;
    data->location |= I2C_RecvByte(1);

    data->power = I2C_RecvByte(1);
    data->power <<= 8;
    data->power |= I2C_RecvByte(1);

    data->sta = I2C_RecvByte(0);

    I2C_Stop();
    return 0;
  }
  return 1;
}


