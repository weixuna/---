#ifndef __BH1750_H__
#define __BH1750_H__
#include "stm32f10x.h"
#include "Delay.h"

/**
 * author: zhenpeng on Tue, 28 May 2024
 * brief: 宏定义GOIO引脚
 * param: void
 * return: void
 **/
#define BH1750_I2C_GPIO_Port GPIOB
#define BH1750_I2C_SCL_GPIOPin GPIO_Pin_6
#define BH1750_I2C_SDA_GPIOPin GPIO_Pin_7
#define BH1750_I2C_SCL(x) GPIO_WriteBit(BH1750_I2C_GPIO_Port, BH1750_I2C_SCL_GPIOPin, (x))
#define BH1750_I2C_SDA(x) GPIO_WriteBit(BH1750_I2C_GPIO_Port, BH1750_I2C_SDA_GPIOPin, (x))
#define BH1750_I2C_SDARead() GPIO_ReadInputDataBit(BH1750_I2C_GPIO_Port, BH1750_I2C_SDA_GPIOPin)
#define HIGH Bit_SET
#define LOW Bit_RESET

/**
 * author: zhenpeng on Tue, 28 May 2024
 * brief: 宏定义BH1750相关地址
 **/
#define BH1750_ADDRESS 0x46
#define BH1750_CMD_AddWrite 0x46
#define BH1750_CMD_AddRead 0x47
#define BH1750_CMD_PowOn 0x01
#define BH1750_CMD_ModeH1 0x10

/**
 * author: zhenpeng on Wed, 21 May 2024
 * brief: 过滤需要用到的函数
 * note: 不在此列的函数，无法在外部调用。为了保证函数在本体的可用性，在源文件内会重新定义一次。
 **/
void BH1750_Init(void);
uint16_t BH1750_FluxValue(void);
uint16_t BH1750_ReadData(void);

#endif
