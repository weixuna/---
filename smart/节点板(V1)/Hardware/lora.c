#include "sys.h"
#include "sensor.h"  // 确保包含传感器头文件
#include <stdbool.h>
// 设备工作模式(用于记录设备状态)
u8 Lora_mode = 0;		// 0:配置模式 1:接收模式 2:发送模式
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
		u3_printf("%s\r\n", cmd); // 发送命令

	if (ack && waittime) // 需要等待应答
	{
		while (--waittime) // 等待倒计时
		{
			delay_ms(10);
			if (USART1_RX_STA & 0X8000) // 接收到期待的应答结果
			{
				if (lora_check_cmd(ack))
					break; // 得到有效数据
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 使能复用功能时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		  // LORA_MD0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  // 推挽输出 ，IO口速度为50MHz

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		  // LORA_AUX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	  // 下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  // 根据设定参数初始化GPIOA.4

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;			   // 中断线关闭(先关闭后面再打开)
	EXTI_Init(&EXTI_InitStructure);						   // 根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // 抢占优先级2，
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;			 // 关闭外部中断通道（后面再打开）
	NVIC_Init(&NVIC_InitStructure);

	LORA_MD0 = 0;
	LORA_AUX = 0;

	while (LORA_AUX) // 确保LORA模块在空闲状态下(LORA_AUX=0)
	{
		delay_ms(500);
	}
	usart1_init(115200); // 初始化串口1

	LORA_MD0 = 1; // 进入AT模式
	delay_ms(40);
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
	while (LORA_AUX)
		;		  // 等待模块空闲
	LORA_MD0 = 1; // 进入配置模式
	delay_ms(40);
	Lora_mode = 0; // 标记"配置模式"
	lora_send_cmd((u8 *)"AT+ADDR=0,5",(u8 *)"OK",200);//设置波特率115200，无校验 	
	lora_send_cmd((u8 *)"AT+CWMODE=0", (u8 *)"OK", 200);	// 设置为一般模式
	lora_send_cmd((u8 *)"AT+TMODE=0", (u8 *)"OK", 200);		// 设置为透明传输
	lora_send_cmd((u8 *)"AT+TPOWER=3", (u8 *)"OK", 200);	// 设置发射功率20db
	lora_send_cmd((u8 *)"AT+WLRATE=28,3", (u8 *)"OK", 200); // 设置信道为24，空中速率2.4
	lora_send_cmd((u8 *)"AT+WLTIME=0", (u8 *)"OK", 200);	// 设置休眠时间为1S
	lora_send_cmd((u8 *)"AT+UART=7,0", (u8 *)"OK", 200);	// 设置波特率115200，无校验

	LORA_MD0 = 0; // 退出配置,进入通信
	delay_ms(40);
	while (LORA_AUX)
		; // 判断是否空闲(模块会重新配置参数)
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
		Aux_Int(Int_mode);					// 重新设置中断边沿
		EXTI_ClearITPendingBit(EXTI_Line1); // 清除LINE4上的中断标志位
	}
}

void LoRa_SendData(void)
{
    u8 temp[200];  // 扩大缓冲区以适应所有传感器数据 
    // 读取传感器数据
    ReadSGP30Data();
		Soil_Init();         // 土壤传感器初始化
	
		MQ2_Init();  //烟雾传感器
	  uint16_t ppm = GetPPM();
    uint32_t lightValue = BH1750_FluxValue();
		uint32_t soil = Soil_ADValue(10);
		DH11_ReadValue();
		//buzzer_off();
    // 格式化数据到字符串缓冲区
    sprintf((char*)temp, "CO2:%u;TVOC:%u;Light:%lu;Temp:%u;Humi:%u;Soil:%u;PPM:%u",
            CO2Data, TVOCData, lightValue, DH11_Var_Temp, DH11_Var_Humi,soil,ppm);		
    // 通过LoRa发送数据
    u3_printf("%s\r\n", temp);
		OLED_ShowString(1, 1, "CO2:    ");
    OLED_ShowNum(1, 5, soil, 3);
    OLED_ShowString(2, 1, "TVOC:   ");
    OLED_ShowNum(2, 6, TVOCData, 3);
    // 显示光照强度
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
// Lora模块接收数据
void LoRa_ReceData(void)
{
    u16 len = 0;
    char recedata[100]; // 假设最大接收长度为100，根据需要调整
    if (USART1_RX_STA & 0x8000)
    {
        len = USART1_RX_STA & 0x7FFF;
        USART1_RX_BUF[len] = 0; // 添加结束符
        USART1_RX_STA = 0;
				buzzer_init();
        // 将接收到的数据复制到recedata数组
        strcpy(recedata, (char *)USART1_RX_BUF);

        // 判断接收到的数据是否是命令
        if (strncmp(recedata, "PUMON", 5) == 0) //打开水泵
        {					
					relay_on();
					buzzer_turn();
        }
        else if (strncmp(recedata, "PUMOFF", 6) == 0) //关闭
        {
					relay_off();
					buzzer_turn();
        }
        else if (strncmp(recedata, "FANone", 6) == 0) //风扇一档
        {
						speek1=60;
        }
        else if (strncmp(recedata, "FANtwo", 6) == 0) //风扇二档
        {
            speek1=100;
        }
        else if (strncmp(recedata, "FANOFF", 6) == 0) //关闭风扇
        {
            speek1=1;
						
        }
        else if (strncmp(recedata, "AIRON", 5) == 0)  //打开天窗
        {
            speek2=1;
        }
        else if (strncmp(recedata, "AIROFF", 6) == 0) //关闭天窗
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
//		USART1_RX_BUF[len]=0;//添加结束符
//		USART1_RX_STA=0;

//		for(i=0;i<len;i++)
//		{
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
//			USART_SendData(USART1,USART1_RX_BUF[i]); //通过串口3接收到的数据发送至串口1，USART3_RX_BUF[i]为数据
//		}
//  }
//}
