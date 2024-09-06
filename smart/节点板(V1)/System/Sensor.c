#include "sys.h"
#include "sensor.h" // 确保包含相应的头文件
uint8_t KeyNum;
uint8_t keyPressed = 0;     // 状态标志，0表示按键未按下，1表示按键已按下
uint32_t CO2Data, TVOCData; // 定义 CO2 和 TVOC 变量
uint32_t sgp30_dat;
uint8_t time = 0;

void InitSystem(void)
{
    OLED_Init();         // 初始化OLED
    //LED_Init();          // LED初始化
    Key_Init();          // 按键初始化
    relay_init();        // 继电器初始化
    SGP30_Init();        // 初始化 SGP30
    BH1750_Init();       // 光强传感器初始化
}
void HandleKeyInput(void)
{
    KeyNum = Key_GetNum(); // 获取按键键码

    if (KeyNum == 1)
    {                   // 按键1按下
        keyPressed = 1; // 设置状态标志
        delay_ms(10);   // 去抖动延时
    }

    if (keyPressed == 1 && KeyNum == 0)
    {                   // 按键释放
        keyPressed = 0; // 清除状态标志
        relay_turn();   // 关闭继电器
    }
}

void WaitForSGP30Stable(void)
{
    // 初始化 SGP30
    SGP30_Init();
    SGP30_Write(0x20, 0x08); // 开启 SGP30
    delay_ms(1000);          // 延时等待 SGP30 稳定

    // 初次读取 SGP30 数据
    sgp30_dat = SGP30_Read();
    CO2Data = (sgp30_dat & 0xffff0000) >> 16;
    TVOCData = sgp30_dat & 0x0000ffff;

    while (CO2Data == 400 && TVOCData == 0)
    {
        SGP30_Write(0x20, 0x08);
        sgp30_dat = SGP30_Read();
        CO2Data = (sgp30_dat & 0xffff0000) >> 16; // 取出 CO2 浓度值
        TVOCData = sgp30_dat & 0x0000ffff;        // 取出 TVOC 值
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
    CO2Data = (sgp30_dat & 0xffff0000) >> 16; // 取出 CO2 浓度值
    TVOCData = sgp30_dat & 0x0000ffff;        // 取出 TVOC 值
}



