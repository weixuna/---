#include "sys.h"
#include "sensor.h" // ȷ��������Ӧ��ͷ�ļ�
uint8_t KeyNum;
uint8_t keyPressed = 0;     // ״̬��־��0��ʾ����δ���£�1��ʾ�����Ѱ���
uint32_t CO2Data, TVOCData; // ���� CO2 �� TVOC ����
uint32_t sgp30_dat;
uint8_t time = 0;

void InitSystem(void)
{
    OLED_Init();         // ��ʼ��OLED
    //LED_Init();          // LED��ʼ��
    Key_Init();          // ������ʼ��
    relay_init();        // �̵�����ʼ��
    SGP30_Init();        // ��ʼ�� SGP30
    BH1750_Init();       // ��ǿ��������ʼ��
}
void HandleKeyInput(void)
{
    KeyNum = Key_GetNum(); // ��ȡ��������

    if (KeyNum == 1)
    {                   // ����1����
        keyPressed = 1; // ����״̬��־
        delay_ms(10);   // ȥ������ʱ
    }

    if (keyPressed == 1 && KeyNum == 0)
    {                   // �����ͷ�
        keyPressed = 0; // ���״̬��־
        relay_turn();   // �رռ̵���
    }
}

void WaitForSGP30Stable(void)
{
    // ��ʼ�� SGP30
    SGP30_Init();
    SGP30_Write(0x20, 0x08); // ���� SGP30
    delay_ms(1000);          // ��ʱ�ȴ� SGP30 �ȶ�

    // ���ζ�ȡ SGP30 ����
    sgp30_dat = SGP30_Read();
    CO2Data = (sgp30_dat & 0xffff0000) >> 16;
    TVOCData = sgp30_dat & 0x0000ffff;

    while (CO2Data == 400 && TVOCData == 0)
    {
        SGP30_Write(0x20, 0x08);
        sgp30_dat = SGP30_Read();
        CO2Data = (sgp30_dat & 0xffff0000) >> 16; // ȡ�� CO2 Ũ��ֵ
        TVOCData = sgp30_dat & 0x0000ffff;        // ȡ�� TVOC ֵ
        OLED_ShowString(1, 1, "Testing...");
        time++;
        OLED_ShowNum(1, 10, time, 3);
        delay_ms(500);
    }
    OLED_Clear();
}

void ReadSGP30Data(void)
{
    SGP30_Write(0x20, 0x08);
    sgp30_dat = SGP30_Read();
    CO2Data = (sgp30_dat & 0xffff0000) >> 16; // ȡ�� CO2 Ũ��ֵ
    TVOCData = sgp30_dat & 0x0000ffff;        // ȡ�� TVOC ֵ
}



