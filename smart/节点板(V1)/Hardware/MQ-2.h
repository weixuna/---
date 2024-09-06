#ifndef MQ_2_H
#define MQ_2_H

#define MQ2_Port		GPIOA
#define MQ2_Pin			GPIO_Pin_7
#define MQ2_ADC_Port	ADC1

#define MQ2_ReadTime	10

void MQ2_Init(void);
uint16_t AD_GetValue_MQ2(void);
uint16_t AD_Average_Data(void);
float Get_Vol(void);
float GetPPM(void);

#endif
