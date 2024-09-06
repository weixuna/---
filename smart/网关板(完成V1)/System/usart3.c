#include "stm32f10x.h" // Device header
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "usart3.h"
#include "LED.h"
#include "lora.h"

extern uint8_t Lora_mode;
static uint8_t Send_buf[10] = {0}; // 更改数据类型为uint8_t
uint8_t USART3_RX_STA = 0;         // 接收状态
uint8_t USART3_RX_CMD;             // 接收保存的数据

// 初始化串口三
void USART3_Init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;   // GPIO结构体变量
    USART_InitTypeDef USART_InitStructure; // 串口结构体变量
    NVIC_InitTypeDef NVIC_InitStructure;   // 中断结构体变量

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // 使能USART3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // 使能GPIOB时钟| RCC_APB2Periph_AFIO
    USART_DeInit(USART3);
    // USART3_TX   GPIOB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;        // PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // 初始化GPIOB.10

    // USART3_RX   GPIOB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            // PB.11
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // 速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);                // 初始化GPIOB.11

    // USART3 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                                     // 波特率一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // 字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // 一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // 收发模式
    USART_Init(USART3, &USART_InitStructure);                                       // 初始化串口3

    USART_Cmd(USART3, ENABLE); // 使能串口

    // 使能接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 开启中断

    // USART3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         // 串口3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断
    NVIC_Init(&NVIC_InitStructure);                           // 初始化NVIC寄存器
}
#define RX_BUFFER_SIZE 6
uint8_t RxBuffer[RX_BUFFER_SIZE];
uint8_t rxCount = 0;
uint16_t RxBufferHead = 0; // 指向下一个将要写入的缓冲区位置
uint16_t RxBufferTail = 0; // 指向最后一个有效数据的位置
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // 接收到数据
    {
        // 如果缓冲区未满，则将接收到的数据存入缓冲区
        if (rxCount <= 5)
        {
            USART_SendData(USART3, 0x15);
            RxBuffer[rxCount] = USART_ReceiveData(USART3); // 读取并清除接收寄存器中的数据
            USART_SendData(USART3, RxBuffer[rxCount]);
        }
        rxCount++;
        // 清除中断标志
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

extern uint16_t CO2;
extern uint16_t TVOC;
extern uint16_t light;
extern uint16_t temp;
extern uint16_t humi;
extern uint16_t soil;

void Process_USART3_Command(void)
{
    if (rxCount > 5)
    {
        switch (RxBuffer[3])
        {
        case 0x01:
            LED1_OFF(); // 关闭LED
            break;
        case 0x02:
            LED1_ON(); // 打开LED
            break;
        case 0x03: // 打开水泵
            if (!LORA_AUX)
            {
                Lora_mode = 2;
                LoRa_SendData("PUMOFF");
            }
        case 0x04: // 关闭水泵
            if (!LORA_AUX)
            {
                Lora_mode = 2;
                LoRa_SendData("PUMON");
            }
        case 0x05: // 打开风扇
            if (!LORA_AUX)
            {
                Lora_mode = 2;
                LoRa_SendData("fanon");
            }
        case 0x06: // 风扇一档
            if (!LORA_AUX)
            {
                Lora_mode = 2;
                LoRa_SendData("FANone");
            }
        case 0x07: // 风扇二档
            if (!LORA_AUX)
            {
                Lora_mode = 2;
                LoRa_SendData("FANtwo");
            }
        case 0x08: // 风扇三档
            if (!LORA_AUX)
            {
                Lora_mode = 2;
                LoRa_SendData("fanthree");
            }
        case 0x09: // 关闭风扇
            if (!LORA_AUX)
            {
                LORA_MD0 = 2;
                LoRa_SendData("FANOFF");
            }
        case 0x10: // 打开天窗
            if (!LORA_AUX)
            {
                Lora_mode = 2;
                LoRa_SendData("AIRON");
            }
        case 0x11: // 关闭天窗
            if (!LORA_AUX)
            {
                Lora_mode = 2;
                LoRa_SendData("AIROFF");
            }
        case 0x12:
            USART3_SendTemperature(0x01, temp); // 发送温度
            break;
        case 0x13:
            USART3_SendTemperature(0x02, humi); // 发送湿度
            break;
        case 0x14:
            USART3_SendTemperature(0x03, soil); // 发送土壤湿度
            break;
        case 0x15:
            USART3_SendTemperature(0x04, CO2); // 发送CO2
            break;
        case 0x16:
            USART3_SendTemperature(0x05, light); // 发送光强
            break;
        default:
            break;
        }
        rxCount = 0;
    }
}

void USART3_SendCmd(uint8_t CMD, uint8_t dat)
{
    Send_buf[0] = 0xAA;
    Send_buf[1] = 0x55;
    Send_buf[2] = CMD;
    Send_buf[3] = dat;
    Send_buf[4] = 0x55;
    Send_buf[5] = 0xAA;
    for (int i = 0; i < 6; i++)
    {
        USART3_SendByte(Send_buf[i]);
    }
}

void USART3_SendTemperature(uint8_t msg_id, uint16_t temp_int)
{
    uint8_t Send_buf1[9];
    Send_buf1[0] = 0xAA;
    Send_buf1[1] = 0x55;
    Send_buf1[2] = msg_id;
    Send_buf1[3] = (uint8_t)(temp_int & 0xFF);
    Send_buf1[4] = (uint8_t)((temp_int >> 8) & 0xFF);
    Send_buf1[5] = 0x00;
    Send_buf1[6] = 0x00;
    Send_buf1[7] = 0x55;
    Send_buf1[8] = 0xAA;

    for (int i = 0; i < 9; i++)
    {
        USART3_SendByte(Send_buf1[i]);
    }
}

void USART3_SendByte(uint8_t data)
{
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
        ;                         // 等待发送寄存器为空
    USART_SendData(USART3, data); // 发送数据
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
        ; // 等待发送完成
}
void USART3_SendString(const char *str)
{
    while (*str)
    {
        USART3_SendByte((uint8_t)*str++);
    }
}
