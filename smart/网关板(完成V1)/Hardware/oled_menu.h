#ifndef __OLED_MENU_H
#define __OLED_MENU_H
#include "stm32f10x.h" // Device header

typedef struct
{
    u8 current;                      // 当前位置
    u8 back;                         // 向上
    u8 next;                         // 向下
    u8 enter;                        // 确认
    void (*current_operation)(void); // 当前状态应该执行的操作
} menu_table;

void menu_key_set(void);

// 开局动画
void Start_up(void);
// 一级选择菜单
void tempHumi_sensor(void);
void light_sensor(void);
void somke_sensor(void);
void soil_sensor(void);

#endif
