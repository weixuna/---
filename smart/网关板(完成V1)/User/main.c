#include "sys.h"
#include "usart3.h"
extern u8 Lora_mode;
uint16_t speed;

extern volatile uint8_t timer_flag;			  // 定时器

int main(void)
{
	LED_Init();
	OLED_Init();
	OLED_ShowString(2,4,"Wecome LoRa");
	SystemInit();
	delay_init();									// 初始化延时函数
	MyUSART_Init();
	Key_Init();
	HuaweiIot_init(); // 上云初始化
	TIM2_Init(17999, 9999); //2秒
	usart1_init(115200);
	USART3_Init(9600);
	LoRa_Init();
	Lora_set();
	while (1)
	{
		menu_key_set();
		if (timer_flag)
		{					// 检查定时器中断标志位
			timer_flag = 0; // 清除标志位
			HuaweiIot_SentData();
		}
		Process_USART3_Command(); // 语言发送命令函数
		LoRa_ReceData();		  // lora接收函数
	}
}
