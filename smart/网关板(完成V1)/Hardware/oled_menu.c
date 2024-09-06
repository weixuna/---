#include "stm32f10x.h" // Device header
#include "oled_menu.h"
#include "oled.h"
#include "key.h"
#include "Delay.h"
#include "usart3.h"

void (*current_operation)(); // 操作函数指针
u8 func_index = 0;
uint8_t key, count_key2;
// current  back next enter (*current_operation)(void)
// 当前位置 上 下 确认
menu_table table[50] =
    {
        {0, 0, 1, 0, (*Start_up)},
        {1, 1, 2, 0, (*tempHumi_sensor)},
        {3, 3, 3, 0, (*light_sensor)},
        {4, 4, 4, 0, (*somke_sensor)},
        {5, 0, 5, 0, (*soil_sensor)},

};

void menu_key_set(void)
{
    key = Key_GetNum();
    if (key == 1)
    {
        func_index = table[func_index].back; // 获取back索引号
        OLED_Clear();
    }
    if (key == 2)
    {
        func_index = table[func_index].next; // 获取next索引号
        OLED_Clear();
        USART3_SendString("showNext");
    }
    if (key == 3)
    {
        func_index = table[func_index].enter; // 获取enter索引号
        OLED_Clear();
    }
    current_operation = table[func_index].current_operation; // 执行当前索引号所对应的功能函数。
    (*current_operation)();                                  // 执行当前操作函数
}
extern uint16_t CO2;
extern uint16_t TVOC;
extern uint16_t light;
extern uint16_t temp;
extern uint16_t humi;
extern uint16_t soil;
extern float mqppm;
// 开局画面
void Start_up(void)
{
    OLED_ShowString(2, 4, "LoRa");
    OLED_ShowCN(2, 4, 0); // 网
    OLED_ShowCN(2, 5, 1); // 关
    OLED_ShowCN(3, 2, 2); // 欢
    OLED_ShowCN(3, 3, 3); // 迎
    OLED_ShowCN(3, 4, 4); // 使
    OLED_ShowCN(3, 5, 5); // 用
}

// 第一页-温湿度传感器
void tempHumi_sensor(void)
{
    OLED_ShowCN(1, 1, 15);
    OLED_ShowCN(1, 2, 16);
    OLED_ShowCN(1, 3, 17);
    OLED_ShowCN(1, 4, 6);
    OLED_ShowCN(1, 5, 7);
    OLED_ShowCN(1, 6, 8);
    OLED_ShowCN(3, 1, 15);
    OLED_ShowCN(3, 2, 17);
    OLED_ShowString(3, 6, ":");
    OLED_ShowNum(3, 7, temp, 2);
    OLED_ShowCN(4, 1, 16);
    OLED_ShowCN(4, 2, 17);
    OLED_ShowString(4, 6, ":");
    OLED_ShowNum(4, 7, humi, 2);
}

// 第二页-光照传感器
void light_sensor(void)
{
    OLED_ShowCN(1, 1, 23);
    OLED_ShowCN(1, 2, 24);
    OLED_ShowCN(1, 3, 6);
    OLED_ShowCN(1, 4, 7);
    OLED_ShowCN(1, 5, 8);
    OLED_ShowCN(3, 1, 23);
    OLED_ShowCN(3, 2, 24);
    OLED_ShowString(3, 6, ":");
    OLED_ShowNum(3, 7, light, 4);
}

// 第三页-烟雾传感器
void somke_sensor(void)
{
    OLED_ShowCN(1, 1, 25);
    OLED_ShowCN(1, 2, 26);
    OLED_ShowCN(1, 3, 6);
    OLED_ShowCN(1, 4, 7);
    OLED_ShowCN(1, 5, 8);
    if (mqppm == 450)
    {
        OLED_ShowCN(3, 2, 29);
        OLED_ShowCN(3, 3, 20);
        OLED_ShowCN(3, 4, 27);
        OLED_ShowCN(3, 5, 25);
    }
    else
    {
        OLED_ShowCN(3, 2, 29);
        OLED_ShowCN(3, 3, 30);
        OLED_ShowCN(3, 4, 28);
        OLED_ShowCN(3, 5, 25);
    }
}

// 第四页-土壤传感器
void soil_sensor(void)
{
    OLED_ShowCN(1, 1, 21);
    OLED_ShowCN(1, 2, 22);
    OLED_ShowCN(1, 3, 6);
    OLED_ShowCN(1, 4, 7);
    OLED_ShowCN(1, 5, 8);
    OLED_ShowCN(3, 1, 21);
    OLED_ShowCN(3, 2, 22);
    OLED_ShowCN(3, 3, 16);
    OLED_ShowCN(3, 4, 17);
    OLED_ShowString(3, 11, ":");
    OLED_ShowNum(3, 12, soil, 4);
}
