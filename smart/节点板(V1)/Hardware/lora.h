#ifndef __LORA_H
#define __LORA_H
#include "sys.h"


u8 LoRa_Init(void);
void Lora_set(void);
void Aux_Int(u8 mode);
void LoRa_Process(void);
void LoRa_SendData(void);
void LoRa_ReceData(void);


#define BITBAND(addr, bitnum)  ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)  MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOB_ODR_Addr    (GPIOB_BASE+12) // 0x40010C0C
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  // 0x40010C08

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr, n)  // 输出
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr, n)  // 输入

#define LORA_AUX  PBin(1)    //LORA模块状态引脚
#define LORA_MD0  PBout(0)  //LORA模块控制引脚

#endif


