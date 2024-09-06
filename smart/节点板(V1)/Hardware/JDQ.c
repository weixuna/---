#include "JDQ.h"
#include "stm32f10x.h" 

void relay_init(void) //LED��ʼ��
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

void relay_on(void)  //��LED1
{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

void relay_off(void) //�ر�LED1
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}

void relay_turn(void)  //��LED1��״̬ȡ��
{
	if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14) == 0)
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
	else if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14) == 1)
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}
