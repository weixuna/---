#ifndef __USART3_H
#define __USART3_H
#include "sys.h"

#define USART3_GPIO_PIN_TX   GPIO_Pin_10
#define USART3_GPIO_PIN_RX   GPIO_Pin_11


void MY_USART3(void);
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data);
void USART_SendString(USART_TypeDef * USARTx, char *str);
uint8_t USART_ReceiveByte(USART_TypeDef* USARTx);
#endif
