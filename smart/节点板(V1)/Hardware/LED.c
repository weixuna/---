//#include "stm32f10x.h"                  // Device header

//void LED_Init(void) //LED初始化
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	GPIO_SetBits(GPIOB,GPIO_Pin_15);
//}

//void LED1_ON(void)  //打开LED1
//{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
//}

//void LED1_OFF(void) //关闭LED1
//{
//	GPIO_SetBits(GPIOB,GPIO_Pin_15);
//}

//void LED1_Turn(void)  //对LED1的状态取反
//{
//	if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15) == 0)
//		GPIO_SetBits(GPIOB,GPIO_Pin_15);
//	else if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15) == 1)
//		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
//}
