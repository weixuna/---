#include "sys.h"


/**
 * author: zhenpeng on Tue, 4 Jun 2024
 * brief: 定义所有函数
 **/
void DH11_GPIOMode_INPUT(void);
void DH11_GPIOMode_OUTPUT(void);
void DH11_Srart(void);
uint8_t DH11_ReadByte(void);
void DH11_ReadValue(void);

/**
 * author: zhenpeng on Tue, 4 Jun 2024
 * brief: 定义全局变量
 **/
uint8_t DH11_Var_Humi;
uint8_t DH11_Var_Temp;

void DH11_GPIOMode_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(DH11_DATA_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = DH11_DATA_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(DH11_DATA_Port, &GPIO_InitStructure);
}

void DH11_GPIOMode_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(DH11_DATA_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Pin = DH11_DATA_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(DH11_DATA_Port, &GPIO_InitStructure);
}
void DH11_Srart(void)
{
    DH11_GPIOMode_OUTPUT();
    DH11_DATA(HIGH);
    delay_us(30);
    DH11_DATA(LOW);
    delay_ms(20);
    DH11_DATA(HIGH);
    delay_us(30);
    DH11_GPIOMode_INPUT();
}
uint8_t DH11_ReadByte(void)
{
    uint8_t data;
    for (uint8_t i = 0; i < 8; i++)
    {
        while (DH11_DATARead == 0);
        delay_us(30);
        data <<= 1;
        if (DH11_DATARead == 1)
        {
            data |= 1;
        }
        while(DH11_DATARead == 1);
    }
    return data;
}
/**
 * author: zhenpeng on Tue, 4 Jun 2024
 * brief: DH11温湿度值读取
 * param: void
 * return: void
 * note: 没有Init函数亦无需初始化使用, 直接循环执行;
         读取全局变量DH11_Var_Humi和DH11_Var_Temp即可;
 **/
void DH11_ReadValue(void)
{
    DH11_Srart();
    DH11_DATA(HIGH);
    if (DH11_DATARead == 0)
    {
        while (DH11_DATARead == 0);
        while (DH11_DATARead == 1);
        uint8_t RH = DH11_ReadByte();
        uint8_t RL = DH11_ReadByte();
        uint8_t TH = DH11_ReadByte();
        uint8_t TL = DH11_ReadByte();
        uint8_t sum = DH11_ReadByte();
        DH11_DATA(LOW);
        delay_us(55);
        DH11_DATA(HIGH);
        
        if (RH+RL+TH+TL == sum)
        {
            DH11_Var_Humi = RH;
            DH11_Var_Temp = TH;
        }
    }
}
