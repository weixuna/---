#ifndef __DH11_H__
#define __DH11_H__

#include "stm32f10x.h"
#include "Delay.h"

/**
 * author: zhenpeng on Tue, 4 Jun 2024
 * brief: �궨��GOIO����
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
 * brief: ������Ҫ�õ��ĺ���
 * note: ���ڴ��еĺ������޷����ⲿ���á�Ϊ�˱�֤�����ڱ���Ŀ����ԣ���Դ�ļ��ڻ����¶���һ�Ρ�
 **/
void DH11_ReadValue(void);

/**
 * author: zhenpeng on Tue, 4 Jun 2024
 * brief: ������Ҫ�õ���ȫ�ֱ���
 * note: ���ڴ��е�ȫ�ֱ�����������޷����ⲿ���á�Ϊ�˱�֤ȫ�ֱ���������ڱ���Ŀ����ԣ���Դ�ļ��ڻ����¶���һ�Ρ�
 **/
extern uint8_t DH11_Var_Humi;
extern uint8_t DH11_Var_Temp;


#endif
