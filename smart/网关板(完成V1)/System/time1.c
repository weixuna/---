#include "sys.h"

volatile uint8_t timer1_27s_flag = 0; // ��ʱ��1 27���жϱ�־λ

// ��ʼ���߼���ʱ��1�����ڶ�ʱ27��
void TIM1_Init(u16 arr, u16 psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    // ���㶨ʱ������ֵ��Ԥ��Ƶֵ��ʵ��27��Ķ�ʱ
    // ����ϵͳʱ��Ϊ72MHz��APB2ʱ��Ϊ72MHz
    // ��ʱ��ʱ��Ϊ72MHz��������Ҫ��ʱ27��
    // ��ʱ��ʱ��Ƶ�� = 72MHz / (psc+1)
    // ��ʱ���������� = (arr+1)
    // ��ʱʱ�� = ��ʱ���������� / ��ʱ��ʱ��Ƶ�� = 27��
    // arr = ��ʱʱ�� * ��ʱ��ʱ��Ƶ�� - 1
    // psc = (��ʱ��ʱ��Ƶ�� - 1) / 1000000 - 1 (ת��Ϊ1MHz�ļ���)
    u32 timer_clock = 72000000; // ��ʱ��ʱ��
    u32 timer_tick_per_second = 1000000; // 1MHz�ļ���Ƶ��
    u32 time_seconds = 27; // ��Ҫ��ʱ������
    u32 time_ticks = time_seconds * timer_tick_per_second; // �ܼ�������
    arr = time_ticks / timer_clock - 1; // ��������ֵ
    psc = (timer_clock / timer_tick_per_second - 1); // ����Ԥ��Ƶֵ
    
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // ���ڸ߼���ʱ��
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    TIM_Cmd(TIM1, ENABLE);
}

// �߼���ʱ��1�жϷ�����
void TIM1_UP_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        timer1_27s_flag = 1; // ���ö�ʱ��1 27���жϱ�־λ
    }
}


