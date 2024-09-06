#include "sys.h"

volatile uint8_t timer1_27s_flag = 0; // 定时器1 27秒中断标志位

// 初始化高级定时器1，用于定时27秒
void TIM1_Init(u16 arr, u16 psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    // 计算定时器重载值和预分频值以实现27秒的定时
    // 假设系统时钟为72MHz，APB2时钟为72MHz
    // 定时器时钟为72MHz，我们需要定时27秒
    // 定时器时钟频率 = 72MHz / (psc+1)
    // 定时器计数周期 = (arr+1)
    // 定时时间 = 定时器计数周期 / 定时器时钟频率 = 27秒
    // arr = 定时时间 * 定时器时钟频率 - 1
    // psc = (定时器时钟频率 - 1) / 1000000 - 1 (转换为1MHz的计数)
    u32 timer_clock = 72000000; // 定时器时钟
    u32 timer_tick_per_second = 1000000; // 1MHz的计数频率
    u32 time_seconds = 27; // 需要定时的秒数
    u32 time_ticks = time_seconds * timer_tick_per_second; // 总计数周期
    arr = time_ticks / timer_clock - 1; // 计算重载值
    psc = (timer_clock / timer_tick_per_second - 1); // 计算预分频值
    
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // 对于高级定时器
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    TIM_Cmd(TIM1, ENABLE);
}

// 高级定时器1中断服务函数
void TIM1_UP_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        timer1_27s_flag = 1; // 设置定时器1 27秒中断标志位
    }
}


