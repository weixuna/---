#ifndef __USART3_H
#define __USART3_H

#include <stdint.h>
#include <stdio.h>
#define USART3_MAX_RECV_LEN		1024			    //�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		600					//����ͻ����ֽ���
extern uint8_t USART3_RX_STA; // ����״̬
extern uint8_t USART3_RX_CMD; // ���ձ��������

void USART3_Init(uint32_t bound);              // ���ڳ�ʼ��
void USART3_IRQHandler(void);                  // �жϺ���
void USART3_SendByte(uint8_t data);            // ���͵��ֽ�����
void USART3_SendSu03T(int len);                // �������ݸ�su03t
void Process_USART3_Command(void);             // ������յ�������
void USART3_DOSUM(int *Str, int len);          // ��ͺ�������
void USART3_SendTemperature(uint8_t msg_id, uint16_t temp_int);
void USART3_SendString(const char *str);
#endif // __USART3_H
