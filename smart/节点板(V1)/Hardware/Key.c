#include "stm32f10x.h"                  // Device header
#include "delay.h"


void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 1)
	{
		delay_ms(20); //对按键按下时进行消抖
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0); //如果按键一直按下，
		//那就一直会卡在这个while循环中，直到按键松开
		delay_ms(20); //对按键松开时进行消抖
		KeyNum = 1; //按键1按下时KeyNum值改为1
	}
	
	return KeyNum;
}
