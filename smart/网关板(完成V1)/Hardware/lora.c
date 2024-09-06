#include "sys.h"
#include <stdbool.h>

// �豸����ģʽ(���ڼ�¼�豸״̬)
u8 Lora_mode = 0;       // 0:����ģʽ 1:����ģʽ 2:����ģʽ
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
    {
        u3_printf("%s\r\n", cmd); // ��������
    }

    if (ack && waittime)
    { // ��Ҫ�ȴ�Ӧ��
        while (--waittime)
        { // �ȴ�����ʱ
            delay_ms(10);
            if (USART1_RX_STA & 0X8000)
            { // ���յ��ڴ���Ӧ����
                if (lora_check_cmd(ack))
                {
                    break; // �õ���Ч����
                }
                USART1_RX_STA = 0;
            }
        }
        if (waittime == 0)
        {
            res = 1;
        }
    }
    return res;
}

u8 LoRa_Init(void)
{
    u8 retry = 0;
    u8 temp = 1;

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // ʹ�ܸ��ù���ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;         // LORA_MD0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // ������� ��IO���ٶ�Ϊ50MHz

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;         // LORA_AUX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;     // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // �����趨������ʼ��GPIOA.4

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // �����ش���
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;             // �ж��߹ر�(�ȹرպ����ٴ�)
    EXTI_Init(&EXTI_InitStructure);                        // ����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // ��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;        // �����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;             // �ر��ⲿ�ж�ͨ���������ٴ򿪣�
    NVIC_Init(&NVIC_InitStructure);

    LORA_MD0 = 0;
    LORA_AUX = 0;

    while (LORA_AUX) // ȷ��LORAģ���ڿ���״̬��(LORA_AUX=0)
    {
        delay_ms(500);
    }
    usart1_init(115200); // ��ʼ������1

    LORA_MD0 = 1; // ����ATģʽ
    delay_ms(40); // �ȴ�ģʽ�л�

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
    LORA_MD0 = 1; // ��������ģʽ
    delay_ms(40);

    Lora_mode = 0; // ���"����ģʽ"

    lora_send_cmd((u8 *)"AT+ADDR=0,5", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+CWMODE=0", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+TMODE=0", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+TPOWER=3", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+WLRATE=28,3", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+WLTIME=0", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+UART=7,0", (u8 *)"OK", 200);
    LORA_MD0 = 0; // �˳�����,����ͨ��
    delay_ms(40);
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
        Aux_Int(Int_mode);                  // ���������жϱ���
        EXTI_ClearITPendingBit(EXTI_Line1); // ���LINE4�ϵ��жϱ�־λ
    }
}

void LoRa_SendData(const char *data)
{
    u8 len = strlen(data); // �����ַ�������
    // ��鴮���Ƿ����
    while ((USART1->SR & 0X40) == 0)
        ;
    u3_printf("%s\r\n", data); // ��������
}

uint16_t CO2 = 0;
uint16_t TVOC = 0;
uint16_t light = 0;
uint16_t temp = 0;
uint16_t humi = 0;
uint16_t soil = 0;
float mqppm=0;
void LoRa_ReceData(void)
{
    u16 len = 0;
    if (USART1_RX_STA & 0x8000)
    {
        len = USART1_RX_STA & 0X7FFF;
        USART1_RX_BUF[len] = '\0'; // ��ӽ�����
        USART1_RX_STA = 0;

        // ��ȡ����������
        sscanf((const char *)USART1_RX_BUF, "CO2:%hu;TVOC:%hu;Light:%hu;Temp:%hu;Humi:%hu;Soil:%hu;PPM:%f%%", &CO2, &TVOC, &light, &temp, &humi, &soil,&mqppm);
    }
}

