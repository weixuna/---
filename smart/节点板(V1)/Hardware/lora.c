#include "sys.h"
#include "sensor.h"  // ȷ������������ͷ�ļ�
#include <stdbool.h>
// �豸����ģʽ(���ڼ�¼�豸״̬)
u8 Lora_mode = 0;		// 0:����ģʽ 1:����ģʽ 2:����ģʽ
static u8 Int_mode = 0; // 0:�ر� 1:������ 2:�½���

EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

// lora���������,�����յ���Ӧ��
// str:�ڴ���Ӧ����
// ����ֵ:0,û�еõ��ڴ���Ӧ����
// ����,�ڴ�Ӧ������λ��(str��λ��)
u8 *lora_check_cmd(u8 *str)
{
	char *strx = 0;
	if (USART1_RX_STA & 0X8000) // ���յ�һ��������
	{
		USART1_RX_BUF[USART1_RX_STA & 0X7FFF] = 0; // ��ӽ�����
		strx = strstr((const char *)USART1_RX_BUF, (const char *)str);
	}
	return (u8 *)strx;
}

// lora��������
// cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
// ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
// waittime:�ȴ�ʱ��(��λ:10ms)
// ����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//        1,����ʧ��
u8 lora_send_cmd(u8 *cmd, u8 *ack, u16 waittime)
{
	u8 res = 0;
	USART1_RX_STA = 0;
	if ((u32)cmd <= 0XFF)
	{
		while ((USART1->SR & 0X40) == 0)
			; // �ȴ���һ�����ݷ������
		USART1->DR = (u32)cmd;
	}
	else
		u3_printf("%s\r\n", cmd); // ��������

	if (ack && waittime) // ��Ҫ�ȴ�Ӧ��
	{
		while (--waittime) // �ȴ�����ʱ
		{
			delay_ms(10);
			if (USART1_RX_STA & 0X8000) // ���յ��ڴ���Ӧ����
			{
				if (lora_check_cmd(ack))
					break; // �õ���Ч����
				USART1_RX_STA = 0;
			}
		}
		if (waittime == 0)
			res = 1;
	}
	return res;
}

u8 LoRa_Init(void)
{
	u8 retry = 0;
	u8 temp = 1;

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PA�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // ʹ�ܸ��ù���ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		  // LORA_MD0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  // ������� ��IO���ٶ�Ϊ50MHz

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		  // LORA_AUX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	  // ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  // �����趨������ʼ��GPIOA.4

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // �����ش���
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;			   // �ж��߹ر�(�ȹرպ����ٴ�)
	EXTI_Init(&EXTI_InitStructure);						   // ����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // ��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 // �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;			 // �ر��ⲿ�ж�ͨ���������ٴ򿪣�
	NVIC_Init(&NVIC_InitStructure);

	LORA_MD0 = 0;
	LORA_AUX = 0;

	while (LORA_AUX) // ȷ��LORAģ���ڿ���״̬��(LORA_AUX=0)
	{
		delay_ms(500);
	}
	usart1_init(115200); // ��ʼ������1

	LORA_MD0 = 1; // ����ATģʽ
	delay_ms(40);
	retry = 3;
	while (retry--)
	{
		if (!lora_send_cmd("AT", "OK", 70))
		{
			temp = 0; // ���ɹ�
			break;
		}
	}
	if (retry == 0)
		temp = 1; // ���ʧ��
	return temp;
}

// ����LORA���ŵ������ʡ����ڡ����书�ʡ�����ģʽ��
void Lora_set(void)
{
	usart1_init(115200);
	usart1_rx(1); // ��������3����
	while (LORA_AUX)
		;		  // �ȴ�ģ�����
	LORA_MD0 = 1; // ��������ģʽ
	delay_ms(40);
	Lora_mode = 0; // ���"����ģʽ"
	lora_send_cmd((u8 *)"AT+ADDR=0,5",(u8 *)"OK",200);//���ò�����115200����У�� 	
	lora_send_cmd((u8 *)"AT+CWMODE=0", (u8 *)"OK", 200);	// ����Ϊһ��ģʽ
	lora_send_cmd((u8 *)"AT+TMODE=0", (u8 *)"OK", 200);		// ����Ϊ͸������
	lora_send_cmd((u8 *)"AT+TPOWER=3", (u8 *)"OK", 200);	// ���÷��书��20db
	lora_send_cmd((u8 *)"AT+WLRATE=28,3", (u8 *)"OK", 200); // �����ŵ�Ϊ24����������2.4
	lora_send_cmd((u8 *)"AT+WLTIME=0", (u8 *)"OK", 200);	// ��������ʱ��Ϊ1S
	lora_send_cmd((u8 *)"AT+UART=7,0", (u8 *)"OK", 200);	// ���ò�����115200����У��

	LORA_MD0 = 0; // �˳�����,����ͨ��
	delay_ms(40);
	while (LORA_AUX)
		; // �ж��Ƿ����(ģ����������ò���)
	USART1_RX_STA = 0;
	Lora_mode = 1; // ���"����ģʽ"
	usart1_init(115200);
	Aux_Int(1); // ����LORA_AUX�������ж�
}

// AUX�ж�����
// mode:���õ�ģʽ 0:�ر� 1:������ 2:�½���
void Aux_Int(u8 mode)
{
	if (!mode)
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE; // �ر��ж�
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	}
	else
	{
		if (mode == 1)
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // ������
		else if (mode == 2)
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½���

		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	}
	Int_mode = mode; // ��¼�ж�ģʽ
	EXTI_Init(&EXTI_InitStructure);
	NVIC_Init(&NVIC_InitStructure);
}

// LORA_AUX�жϷ�����
void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1))
	{
		if (Int_mode == 1) // ������(����:��ʼ�������� ����:���ݿ�ʼ���)
		{
			if (Lora_mode == 1) // ����ģʽ
			{
				USART1_RX_STA = 0; // ���ݼ�����0
			}
			Int_mode = 2; // �����½��ش���
		}
		else if (Int_mode == 2) // �½���(����:�����ѷ����� ����:�����������)
		{
			if (Lora_mode == 1) // ����ģʽ
			{
				USART1_RX_STA |= 1 << 15; // ���ݼ���������
			}
			else if (Lora_mode == 2) // ����ģʽ(�������ݷ������)
			{
				Lora_mode = 1; // �������ģʽ
			}
			Int_mode = 1; // ���������ش���
		}
		Aux_Int(Int_mode);					// ���������жϱ���
		EXTI_ClearITPendingBit(EXTI_Line1); // ���LINE4�ϵ��жϱ�־λ
	}
}

void LoRa_SendData(void)
{
    u8 temp[200];  // ���󻺳�������Ӧ���д��������� 
    // ��ȡ����������
    ReadSGP30Data();
		Soil_Init();         // ������������ʼ��
	
		MQ2_Init();  //��������
	  uint16_t ppm = GetPPM();
    uint32_t lightValue = BH1750_FluxValue();
		uint32_t soil = Soil_ADValue(10);
		DH11_ReadValue();
		//buzzer_off();
    // ��ʽ�����ݵ��ַ���������
    sprintf((char*)temp, "CO2:%u;TVOC:%u;Light:%lu;Temp:%u;Humi:%u;Soil:%u;PPM:%u",
            CO2Data, TVOCData, lightValue, DH11_Var_Temp, DH11_Var_Humi,soil,ppm);		
    // ͨ��LoRa��������
    u3_printf("%s\r\n", temp);
		OLED_ShowString(1, 1, "CO2:    ");
    OLED_ShowNum(1, 5, soil, 3);
    OLED_ShowString(2, 1, "TVOC:   ");
    OLED_ShowNum(2, 6, TVOCData, 3);
    // ��ʾ����ǿ��
    OLED_ShowString(3, 1, "Light:  ");
    OLED_ShowNum(3, 8,lightValue, 4);
		DH11_ReadValue();
    OLED_ShowString(4, 1, "Temp:   ");
    OLED_ShowNum(4, 6, DH11_Var_Temp, 2);
    OLED_ShowString(4, 9, "Humi:   ");
    OLED_ShowNum(4, 15,DH11_Var_Humi, 2);  
}



/*
					PUMON PUMON PANON PANone PANtwo PANthree PANOFF AIRON AIROFF

*/
uint16_t speek1;
uint16_t speek2;
// Loraģ���������
void LoRa_ReceData(void)
{
    u16 len = 0;
    char recedata[100]; // ���������ճ���Ϊ100��������Ҫ����
    if (USART1_RX_STA & 0x8000)
    {
        len = USART1_RX_STA & 0x7FFF;
        USART1_RX_BUF[len] = 0; // ��ӽ�����
        USART1_RX_STA = 0;
				buzzer_init();
        // �����յ������ݸ��Ƶ�recedata����
        strcpy(recedata, (char *)USART1_RX_BUF);

        // �жϽ��յ��������Ƿ�������
        if (strncmp(recedata, "PUMON", 5) == 0) //��ˮ��
        {					
					relay_on();
					buzzer_turn();
        }
        else if (strncmp(recedata, "PUMOFF", 6) == 0) //�ر�
        {
					relay_off();
					buzzer_turn();
        }
        else if (strncmp(recedata, "FANone", 6) == 0) //����һ��
        {
						speek1=60;
        }
        else if (strncmp(recedata, "FANtwo", 6) == 0) //���ȶ���
        {
            speek1=100;
        }
        else if (strncmp(recedata, "FANOFF", 6) == 0) //�رշ���
        {
            speek1=1;
						
        }
        else if (strncmp(recedata, "AIRON", 5) == 0)  //���촰
        {
            speek2=1;
        }
        else if (strncmp(recedata, "AIROFF", 6) == 0) //�ر��촰
        {
            speek2=456;
        }else if(strncmp(recedata,"Buzzer",6)==0){
						
				}
    }
}
//void LoRa_ReceData(void)
//{		u16 i=0;
//		u16 len=0;
//  if(USART1_RX_STA&0x8000)
//	{
//		len = USART1_RX_STA&0X7FFF;
//		USART1_RX_BUF[len]=0;//��ӽ�����
//		USART1_RX_STA=0;

//		for(i=0;i<len;i++)
//		{
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������
//			USART_SendData(USART1,USART1_RX_BUF[i]); //ͨ������3���յ������ݷ���������1��USART3_RX_BUF[i]Ϊ����
//		}
//  }
//}
