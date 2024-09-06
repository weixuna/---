#include "sys.h"
#include "sensor.h" // ȷ��������Ӧ��ͷ�ļ�
extern u8 Lora_mode;

extern volatile uint8_t timer_flag;			  // ���ͺ�����־λ
extern volatile uint8_t timer_flag_ReceData;	
extern volatile uint8_t timer1_27s_flag; // ��ʱ��1 27���жϱ�־λ
extern uint16_t speek1;
extern uint16_t speek2;

int main(void)
{

	delay_init();									// ��ʼ����ʱ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����
	usart1_init(115200);
	MY_USART3();
	LoRa_Init();
	Lora_set();
	TIM1_Init(65535, 15456);//��Լ27��
	InitSystem(); // ��ʼ��ϵͳ
	WaitForSGP30Stable();
	TIM2_Init(35999, 9999); // ��Լ5��
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
            Motor_SetSpeed1(100); // ���õ���ٶ�Ϊ50
            // ������ʱ��
            TIM_Cmd(TIM1, ENABLE); // ʹ�ܶ�ʱ��1
            // �ȴ���ʱ���жϱ�־λ
            while (!timer1_27s_flag);
            // ��ʱ���жϱ�־λ����λ��27���ѹ�
            Motor_SetSpeed1(0); // ֹͣ���
            timer1_27s_flag = 0; // �����ʱ���жϱ�־λ
            // �رն�ʱ�����ȴ���һ�������ź�
            TIM_Cmd(TIM1, DISABLE);
						speek2 = 0; // �ı�speek2��ֵ�������ظ�����
        } else if (speek2 == 456) {
            // ������������߼����������ò�ͬ���ٶ�
            Motor_SetSpeed1(-100); // ���õ���ٶ�Ϊ-100
            // ������ʱ��
            TIM_Cmd(TIM1, ENABLE);
            // �ȴ���ʱ���жϱ�־λ
            while (!timer1_27s_flag);
            // ��ʱ���жϱ�־λ����λ��27���ѹ�
            Motor_SetSpeed1(0); // ֹͣ���
            timer1_27s_flag = 0; // �����ʱ���жϱ�־λ
            // �رն�ʱ�����ȴ���һ�������ź�
            TIM_Cmd(TIM1, DISABLE);
						speek2 = 0; // �ı�speek2��ֵ�������ظ�����
        }
		
		LoRa_ReceData(); // �������ݺ���
		
		if (timer_flag)
		{					// ��鶨ʱ���жϱ�־λ
			timer_flag = 0; // �����־λ
			if (!LORA_AUX)	// ������
			{
				Lora_mode = 2;	 // ���"����״̬"
				LoRa_SendData(); // ��������
			}
		}
		
	}
}

