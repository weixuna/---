#include "stm32f10x.h"                  // Device header
#include "MQ-2.h"
#include "Delay.h"
#include "math.h"

#define RL	10
#define R0	26

// 使用 ADC1_IN7(PA7)
void MQ2_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = MQ2_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MQ2_Port,&GPIO_InitStructure);
    
    ADC_RegularChannelConfig(MQ2_ADC_Port, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
    
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_Init(MQ2_ADC_Port,&ADC_InitStructure);
    
    ADC_Cmd(MQ2_ADC_Port,ENABLE);
    
    ADC_ResetCalibration(MQ2_ADC_Port);                                // 固定流程，内部有电路会自动执行校准
    while (ADC_GetResetCalibrationStatus(MQ2_ADC_Port) == SET);
    ADC_StartCalibration(MQ2_ADC_Port);
    while (ADC_GetCalibrationStatus(MQ2_ADC_Port) == SET);
    
    ADC_SoftwareStartConvCmd(MQ2_ADC_Port, ENABLE);
}

uint16_t AD_GetValue_MQ2(void)
{
    while (ADC_GetFlagStatus(MQ2_ADC_Port, ADC_FLAG_EOC) == RESET);    // 等待 EOC 标志位，即等待 AD 转换结束
    return ADC_GetConversionValue(MQ2_ADC_Port);                    // 读数据寄存器，得到 AD 转换的结果
}

// 取平均值
uint16_t AD_Average_Data(void)
{
    uint16_t Aver_Value = 0;
    for(int i = 0; i < MQ2_ReadTime; i++)
    {
        Aver_Value += AD_GetValue_MQ2();
        delay_ms(5);
    }
    Aver_Value /= MQ2_ReadTime;
    return Aver_Value;
}

// 电压
float Get_Vol(void)
{
    uint16_t ADC_Value;
    float voltage = 0;
    
    ADC_Value = AD_Average_Data();
    delay_ms(5);
    
    voltage = (3.3 / 4096.0) * (ADC_Value);
    
    return voltage;
}

// 浓度
float GetPPM(void)
{
    float RS = (3.3f - Get_Vol()) / Get_Vol() * RL;
    float ppm = 98.322f * pow(RS / R0, -1.458f);
    return ppm;
}
