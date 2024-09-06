#include "stm32f10x.h" // Device header
#include <string.h>
#include <stdio.h>
#include "iot.h"
#include "led.h"

uint8_t uart2_rec_i = 0;
extern uint8_t atok_rec_flag;
char uart2_recdata = 0, uart2_recstring[256], sub_string[150], analysis_Str[256];
extern uint16_t speed;
uint16_t speed_flag;

void MyUSART_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

// *********************************************************************************************************
// * 函 数 名: USART1_IRQHandler
// * 功能说明: 串口中断1函数：主要完成esp8266模块的通信接收处理
// * 形 参：无
// * 返 回 值: 无
// *********************************************************************************************************
void USART2_IRQHandler(void) // 串口1中断服务程序
{
    char str_temp[128]; //临时字符串
    char request_id[48];

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // 接收中断
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // 清除中断标志
        uart2_recdata = USART_ReceiveData(USART2);      // 读取接收到的数据
        if (uart2_recdata != '\n')
        {
            // 防止第一个接收为空格
            if ((uart2_rec_i == 0) && (uart2_recdata == ' '))
            {
                // 测试发现在接收MQTT服务器下发的命令时，有时接收的数据第一位为空格
                // 通过上述判断方式可以解决，触发原因暂时不明，贾工留坑-2022.9.3.1
            }
            else
            {
                uart2_recstring[uart2_rec_i] = uart2_recdata;
                uart2_rec_i++; // 统计接收字符数量
            }
        }
        else
        {
            if (uart2_rec_i >= 2)
            {
                strncpy(sub_string, uart2_recstring, 2); // 截取前两位
                if (strcmp(sub_string, "OK") == 0)
                {
                    atok_rec_flag = 1;
                }

                strncpy(sub_string, uart2_recstring, 5);
                if (strcmp(sub_string, "ready") == 0)
                {
                    atok_rec_flag = 1;
                }

                strncpy(sub_string, uart2_recstring, 5);
                if (strcmp(sub_string, "ERROR") == 0)
                {
                    atok_rec_flag = 0;
                }
                // 字符串硬解析
                // +MQTTSUBRECV:0,"$oc/devices/66362c827bdccc0126d6c1c3_wulianwang/sys/commands/request_id=c9881dfe-a62b-40ad-9197-43c0f7f5cd4d",68,{"paras":{"led":"1"},"service_id":"Dev_data","command_name":"Control"}
                if (uart2_rec_i >= 200)
                {
                    strncpy(analysis_Str, uart2_recstring, 12); // 提取出“+MQTTSUBRECV”
                    if (strcmp(analysis_Str, "+MQTTSUBRECV") == 0)
                    {
                        memset(analysis_Str, 0, sizeof(analysis_Str)); // 清空缓存区
                        // 提取request_id
                        char *request_id_start = strstr(uart2_recstring, "request_id=") + 11; // request_id 的起始位置
                        if (request_id_start != NULL)
                        {
                            strncpy(request_id, request_id_start, 36); // 提取出 request_id
                            request_id[36] = '\0';                     // 确保字符串结尾
                            printf("request_id=%s\r\n", request_id);
                        }
                        // 提取paras
                        char *paras_start = strstr(uart2_recstring, "\"paras\":{\"led\":\"");
                        if (paras_start != NULL)
                        {
                            paras_start += 16; //"led" 的起始位置的后一个字符
                            if (paras_start[0] == '0')
                            {
                                LED1_OFF();
                            }
                            else if (paras_start[0] == '1')
                            {
                                LED1_ON();
                            }

                            // 向云平台完成命令响应
                            printf("AT+MQTTPUB=0,\"$oc/devices/%s/sys/commands/response/request_id=%s\",\"\",0,0\r\n", HUAWEI_MQTT_DeviceID, request_id);
                            memset(analysis_Str, 0, sizeof(analysis_Str)); // 清空缓存区
                            memset(str_temp, 0, sizeof(str_temp));         // 清空缓存区
                        }
                    }
                }
            }
            uart2_rec_i = 0;
            memset(uart2_recstring, 0, sizeof(uart2_recstring)); // 清空 uart2_recstring 字符串
            memset(sub_string, 0, sizeof(sub_string));           // 清空 sub_string 字符串
        }
    }
}

// 内部调用函数，注意要勾选OPTIONS中的USE Micro LIB选项
int fputc(int ch, FILE *f) // printf重定向
{
    USART_SendData(USART2, (uint8_t)ch);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        ;
    return ch;
}
