#include "sys.h"

/**
 * author: zhenpeng on Fri,17 May 2024 17:00
 * brief: GPIO初始化函数
 * param: void
 * return: void
 * note: ADC AD模式, 239时间, 通道1
 **/
void Soil_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);  // 开启ADC1和GPIOA时钟

    // GPIO初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        // 配置为模拟输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;            // 指定引脚为GPIO_Pin_1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // 设置IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);               // 初始化GPIOA

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // 设置ADC时钟分频为PCLK2/6
    
    // ADC初始化结构体
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 设置转换触发模式为不使用外部触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 设置数据对齐方式为右对齐
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // 设置工作模式为独立模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 开启连续转换模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       // 关闭扫描模式
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // 设置转换的通道数目为1
    ADC_Init(ADC1, &ADC_InitStructure);                                 // 初始化ADC1
    
    // ADC通道配置
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);

    // 开启ADC电源
    ADC_Cmd(ADC1, ENABLE);

    // 复位校准
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) != RESET);

    // 开始校准
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) != RESET);

    // 启动软件转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

uint16_t AD_GetValue(void)
{
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);  // 等待转换完成
    return ADC_GetConversionValue(ADC1);                     // 返回转换结果
}

/**
 * author: zhenpeng on Fri,17 May 2024 17:00
 * brief: 读取AD值
 * param: void
 * return: uint16_t ADC_GetConversionValue(ADC1): AD的电压值
 **/
uint16_t Soil_ADValue(int times)
{
    uint32_t H_all = 0;
    float H_arg = 0;
    uint8_t t;

    // 进行多次ADC转换并累加
    for (t = 0; t < times; t++) {
        H_all += AD_GetValue();
        delay_ms(1); // 延时1毫秒
    }
    // 计算平均值
    H_arg = (float)H_all / times;
    // 根据转换公式计算湿度值
    uint16_t data = (uint16_t)((4095 - H_arg) / 4095 * 100);
    // 确保湿度值在合理范围内（0~100）
    data = data > 100 ? 100 : data;
    return data;
}
