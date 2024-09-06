#include "sys.h"
#include "sensor.h" // 确保包含相应的头文件
extern u8 Lora_mode;

extern volatile uint8_t timer_flag;			  // 发送函数标志位
extern volatile uint8_t timer_flag_ReceData;	
extern volatile uint8_t timer1_27s_flag; // 定时器1 27秒中断标志位
extern uint16_t speek1;
extern uint16_t speek2;

int main(void)
{

	delay_init();									// 初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置中断优先级分组
	usart1_init(115200);
	MY_USART3();
	LoRa_Init();
	Lora_set();
	TIM1_Init(65535, 15456);//大约27秒
	InitSystem(); // 初始化系统
	WaitForSGP30Stable();
	TIM2_Init(35999, 9999); // 大约5秒
	Key_Init();
	Motor_Init();
	while (1)
	{
		
		if(speek1==60){
				Motor_SetSpeed2(50);
		}else if(speek1==100){
				Motor_SetSpeed2(100);
		}else if(speek1==1){ 
			Motor_SetSpeed2(0);
		}
		
		        if (speek2 == 1) {
            Motor_SetSpeed1(100); // 设置电机速度为50
            // 启动定时器
            TIM_Cmd(TIM1, ENABLE); // 使能定时器1
            // 等待定时器中断标志位
            while (!timer1_27s_flag);
            // 定时器中断标志位已置位，27秒已过
            Motor_SetSpeed1(0); // 停止电机
            timer1_27s_flag = 0; // 清除定时器中断标志位
            // 关闭定时器，等待下一个启动信号
            TIM_Cmd(TIM1, DISABLE);
						speek2 = 0; // 改变speek2的值，避免重复进入
        } else if (speek2 == 456) {
            // 类似于上面的逻辑，但是设置不同的速度
            Motor_SetSpeed1(-100); // 设置电机速度为-100
            // 启动定时器
            TIM_Cmd(TIM1, ENABLE);
            // 等待定时器中断标志位
            while (!timer1_27s_flag);
            // 定时器中断标志位已置位，27秒已过
            Motor_SetSpeed1(0); // 停止电机
            timer1_27s_flag = 0; // 清除定时器中断标志位
            // 关闭定时器，等待下一个启动信号
            TIM_Cmd(TIM1, DISABLE);
						speek2 = 0; // 改变speek2的值，避免重复进入
        }
		
		LoRa_ReceData(); // 接收数据函数
		
		if (timer_flag)
		{					// 检查定时器中断标志位
			timer_flag = 0; // 清除标志位
			if (!LORA_AUX)	// 若空闲
			{
				Lora_mode = 2;	 // 标记"发送状态"
				LoRa_SendData(); // 发送数据
			}
		}
		
	}
}

