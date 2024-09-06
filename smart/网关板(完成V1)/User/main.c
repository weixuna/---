#include "sys.h"
#include "usart3.h"
extern u8 Lora_mode;
uint16_t speed;

extern volatile uint8_t timer_flag;			  // ��ʱ��

int main(void)
{
	LED_Init();
	OLED_Init();
	OLED_ShowString(2,4,"Wecome LoRa");
	SystemInit();
	delay_init();									// ��ʼ����ʱ����
	MyUSART_Init();
	Key_Init();
	HuaweiIot_init(); // ���Ƴ�ʼ��
	TIM2_Init(17999, 9999); //2��
	usart1_init(115200);
	USART3_Init(9600);
	LoRa_Init();
	Lora_set();
	while (1)
	{
		menu_key_set();
		if (timer_flag)
		{					// ��鶨ʱ���жϱ�־λ
			timer_flag = 0; // �����־λ
			HuaweiIot_SentData();
		}
		Process_USART3_Command(); // ���Է��������
		LoRa_ReceData();		  // lora���պ���
	}
}
