#include "sys.h"
#include <stdbool.h>

// 设备工作模式(用于记录设备状态)
u8 Lora_mode = 0;       // 0:配置模式 1:接收模式 2:发送模式
static u8 Int_mode = 0; // 0:关闭 1:上升沿 2:下降沿

EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

// lora发送命令后,检测接收到的应答
// str:期待的应答结果
// 返回值:0,没有得到期待的应答结果
// 其他,期待应答结果的位置(str的位置)
u8 *lora_check_cmd(u8 *str)
{
    char *strx = 0;
    if (USART1_RX_STA & 0X8000) // 接收到一次数据了
    {
        USART1_RX_BUF[USART1_RX_STA & 0X7FFF] = 0; // 添加结束符
        strx = strstr((const char *)USART1_RX_BUF, (const char *)str);
    }
    return (u8 *)strx;
}

// lora发送命令
// cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
// ack:期待的应答结果,如果为空,则表示不需要等待应答
// waittime:等待时间(单位:10ms)
// 返回值:0,发送成功(得到了期待的应答结果)
//        1,发送失败
u8 lora_send_cmd(u8 *cmd, u8 *ack, u16 waittime)
{
    u8 res = 0;
    USART1_RX_STA = 0;
    if ((u32)cmd <= 0XFF)
    {
        while ((USART1->SR & 0X40) == 0)
            ; // 等待上一次数据发送完成
        USART1->DR = (u32)cmd;
    }
    else
    {
        u3_printf("%s\r\n", cmd); // 发送命令
    }

    if (ack && waittime)
    { // 需要等待应答
        while (--waittime)
        { // 等待倒计时
            delay_ms(10);
            if (USART1_RX_STA & 0X8000)
            { // 接收到期待的应答结果
                if (lora_check_cmd(ack))
                {
                    break; // 得到有效数据
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

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 使能复用功能时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;         // LORA_MD0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // 推挽输出 ，IO口速度为50MHz

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;         // LORA_AUX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;     // 下拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // 根据设定参数初始化GPIOA.4

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;             // 中断线关闭(先关闭后面再打开)
    EXTI_Init(&EXTI_InitStructure);                        // 根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // 抢占优先级2，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;        // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;             // 关闭外部中断通道（后面再打开）
    NVIC_Init(&NVIC_InitStructure);

    LORA_MD0 = 0;
    LORA_AUX = 0;

    while (LORA_AUX) // 确保LORA模块在空闲状态下(LORA_AUX=0)
    {
        delay_ms(500);
    }
    usart1_init(115200); // 初始化串口1

    LORA_MD0 = 1; // 进入AT模式
    delay_ms(40); // 等待模式切换

    retry = 3;
    while (retry--)
    {
        if (!lora_send_cmd("AT", "OK", 70))
        {
            temp = 0; // 检测成功
            break;
        }
    }
    if (retry == 0)
        temp = 1; // 检测失败
    return temp;
}

// 配置LORA的信道、速率、串口、发射功率、传输模式等
void Lora_set(void)
{
    usart1_init(115200);
    usart1_rx(1); // 开启串口3接收
    LORA_MD0 = 1; // 进入配置模式
    delay_ms(40);

    Lora_mode = 0; // 标记"配置模式"

    lora_send_cmd((u8 *)"AT+ADDR=0,5", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+CWMODE=0", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+TMODE=0", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+TPOWER=3", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+WLRATE=28,3", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+WLTIME=0", (u8 *)"OK", 200);
    lora_send_cmd((u8 *)"AT+UART=7,0", (u8 *)"OK", 200);
    LORA_MD0 = 0; // 退出配置,进入通信
    delay_ms(40);
    USART1_RX_STA = 0;
    Lora_mode = 1; // 标记"接收模式"
    usart1_init(115200);
    Aux_Int(1); // 设置LORA_AUX上升沿中断
}

// AUX中断设置
// mode:配置的模式 0:关闭 1:上升沿 2:下降沿
void Aux_Int(u8 mode)
{
    if (!mode)
    {
        EXTI_InitStructure.EXTI_LineCmd = DISABLE; // 关闭中断
        NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    }
    else
    {
        if (mode == 1)
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿
        else if (mode == 2)
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿

        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    }
    Int_mode = mode; // 记录中断模式
    EXTI_Init(&EXTI_InitStructure);
    NVIC_Init(&NVIC_InitStructure);
}

// LORA_AUX中断服务函数
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1))
    {
        if (Int_mode == 1) // 上升沿(发送:开始发送数据 接收:数据开始输出)
        {
            if (Lora_mode == 1) // 接收模式
            {
                USART1_RX_STA = 0; // 数据计数清0
            }
            Int_mode = 2; // 设置下降沿触发
        }
        else if (Int_mode == 2) // 下降沿(发送:数据已发送完 接收:数据输出结束)
        {
            if (Lora_mode == 1) // 接收模式
            {
                USART1_RX_STA |= 1 << 15; // 数据计数标记完成
            }
            else if (Lora_mode == 2) // 发送模式(串口数据发送完毕)
            {
                Lora_mode = 1; // 进入接收模式
            }
            Int_mode = 1; // 设置上升沿触发
        }
        Aux_Int(Int_mode);                  // 重新设置中断边沿
        EXTI_ClearITPendingBit(EXTI_Line1); // 清除LINE4上的中断标志位
    }
}

void LoRa_SendData(const char *data)
{
    u8 len = strlen(data); // 计算字符串长度
    // 检查串口是否空闲
    while ((USART1->SR & 0X40) == 0)
        ;
    u3_printf("%s\r\n", data); // 发送数据
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
        USART1_RX_BUF[len] = '\0'; // 添加结束符
        USART1_RX_STA = 0;

        // 提取并更新数据
        sscanf((const char *)USART1_RX_BUF, "CO2:%hu;TVOC:%hu;Light:%hu;Temp:%hu;Humi:%hu;Soil:%hu;PPM:%f%%", &CO2, &TVOC, &light, &temp, &humi, &soil,&mqppm);
    }
}

