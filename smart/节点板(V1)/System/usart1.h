#ifndef __USART1_H
#define __USART1_H
#include "sys.h"

#define USART1_MAX_RECV_LEN		1024			    //�����ջ����ֽ���
#define USART1_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART1_RX_EN 			1					//0,������;1,����.

extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 USART1_RX_STA;   						//��������״̬

void usart1_init(u32 bound);		//����2��ʼ�� 
void usart1_set(u8 bps,u8 parity);    
void usart1_rx(u8 enable);
void u3_printf(char* fmt,...);

#endif
