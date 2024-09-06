#ifndef __Soil_H__
#define __Soil_H__
#include "stm32f10x.h"

void Soil_Init(void);
uint16_t Soil_ADValue(int times);
uint16_t AD_GetValue(void);
#endif
