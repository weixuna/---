#ifndef __DH11_H__
#define __DH11_H__

#include "stm32f10x.h"
#include "Delay.h"

/**
 * author: zhenpeng on Tue, 4 Jun 2024
 * brief: 宏定义GOIO引脚
 * param: void
 * return: void
 **/
#define DH11_DATA_Port GPIOA
#define DH11_DATA_RCC RCC_APB2Periph_GPIOA
#define DH11_DATA_Pin GPIO_Pin_8
#define DH11_DATA(x) GPIO_WriteBit(DH11_DATA_Port, DH11_DATA_Pin, (x))
#define DH11_DATARead GPIO_ReadInputDataBit(DH11_DATA_Port, DH11_DATA_Pin)
#define HIGH Bit_SET
#define LOW Bit_RESET

/**
 * author: zhenpeng on Tue, 4 Jun 2024
 * brief: 过滤需要用到的函数
 * note: 不在此列的函数，无法在外部调用。为了保证函数在本体的可用性，在源文件内会重新定义一次。
 **/
void DH11_ReadValue(void);

/**
 * author: zhenpeng on Tue, 4 Jun 2024
 * brief: 过滤需要用到的全局变量
 * note: 不在此列的全局变量或变量，无法在外部调用。为了保证全局变量或变量在本体的可用性，在源文件内会重新定义一次。
 **/
extern uint8_t DH11_Var_Humi;
extern uint8_t DH11_Var_Temp;


#endif
