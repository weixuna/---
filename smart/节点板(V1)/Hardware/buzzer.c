#include "sys.h" 

void buzzer_init(void) //LED≥ı ºªØ
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_5);
}

void buzzer_on(void) 
{
    GPIO_SetBits(GPIOA, GPIO_Pin_5); 
}

void buzzer_off(void) 
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_5); 
}

void buzzer_turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5) == 0) 
        GPIO_SetBits(GPIOA, GPIO_Pin_5); 
    else if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5) == 1)
        GPIO_ResetBits(GPIOA, GPIO_Pin_5); 
}
