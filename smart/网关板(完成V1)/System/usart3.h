#ifndef __USART3_H
#define __USART3_H

#include <stdint.h>
#include <stdio.h>
#define USART3_MAX_RECV_LEN		1024			    //最大接收缓存字节数
#define USART3_MAX_SEND_LEN		600					//最大发送缓存字节数
extern uint8_t USART3_RX_STA; // 接收状态
extern uint8_t USART3_RX_CMD; // 接收保存的数据

void USART3_Init(uint32_t bound);              // 串口初始化
void USART3_IRQHandler(void);                  // 中断函数
void USART3_SendByte(uint8_t data);            // 发送单字节数据
void USART3_SendSu03T(int len);                // 发送数据给su03t
void Process_USART3_Command(void);             // 处理接收到的命令
void USART3_DOSUM(int *Str, int len);          // 求和函数声明
void USART3_SendTemperature(uint8_t msg_id, uint16_t temp_int);
void USART3_SendString(const char *str);
#endif // __USART3_H
