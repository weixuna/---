#ifndef __HX711_H
#define __HX711_H

#include "stm32f10x.h" // �豸ͷ�ļ�

// ����λ��������
#define BITBAND(addr, bitnum)  ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)  MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOB_ODR_Addr    (GPIOB_BASE+12) // 0x40010C0C
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  // 0x40010C08

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr, n)  // ���
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr, n)  // ����

#define SGP30_SCL   PBout(10)
#define SGP30_SDA   PBout(11)

// USART GPIO ���ź궨��
#define SGP30_SCL_GPIO_CLK        RCC_APB2Periph_GPIOB
#define SGP30_SCL_GPIO_PORT       GPIOB
#define SGP30_SCL_GPIO_PIN        GPIO_Pin_10

#define SGP30_SDA_GPIO_SDA        RCC_APB2Periph_GPIOB
#define SGP30_SDA_GPIO_PORT       GPIOB
#define SGP30_SDA_GPIO_PIN        GPIO_Pin_11

#define SGP30_SDA_READ()           GPIO_ReadInputDataBit(SGP30_SDA_GPIO_PORT, SGP30_SDA_GPIO_PIN)

#define SGP30_read  0xb1  // SGP30 �Ķ���ַ
#define SGP30_write 0xb0  // SGP30 ��д��ַ

void SGP30_IIC_Start(void);       // ���� IIC ��ʼ�ź�
void SGP30_IIC_Stop(void);        // ���� IIC ֹͣ�ź�
void SGP30_IIC_Send_Byte(uint8_t txd);  // IIC ����һ���ֽ�
uint16_t SGP30_IIC_Read_Byte(unsigned char ack); // IIC ��ȡһ���ֽ�
uint8_t SGP30_IIC_Wait_Ack(void);  // IIC �ȴ� ACK �ź�
void SGP30_IIC_Ack(void);        // IIC ���� ACK �ź�
void SGP30_IIC_NAck(void);       // IIC ������ ACK �ź�
void SGP30_IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t SGP30_IIC_Read_One_Byte(uint8_t daddr, uint8_t addr);
void SGP30_Init(void);
void SGP30_Write(uint8_t a, uint8_t b);
uint32_t SGP30_Read(void);

#endif
