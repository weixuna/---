/****************
1、串口时钟、GPIOA时钟使能
2、GPIOA端口模式设置
3、串口参数初始化
4、开启中断并且初始化NVIC
5、使能串口
6、编写中断处理函数
******************/
#include "sys.h"  //包含需要的头文件

extern u8 Lora_mode;
//串口接收缓存区 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 			//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过timer,则认为不是1次连续数据.也就是超过timer没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART1_RX_STA=0;   	

void USART1_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(USART1);		 
		if((USART1_RX_STA&(1<<15))==0)              //接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 
			if(USART1_RX_STA<USART1_MAX_RECV_LEN)	//还可以接收数据
			{			
				if(!Lora_mode)//配置功能下(启动定时器超时)
				{
					TIM_SetCounter(TIM7,0);             //计数器清空          				
					if(USART1_RX_STA==0) 				//使能定时器7的中断 
					{
						TIM_Cmd(TIM7,ENABLE);           //使能定时器7
					}
				}
				USART1_RX_BUF[USART1_RX_STA++]=res;	//记录接收到的值	 
			}else 
			{
				USART1_RX_STA|=1<<15;				//强制标记接收完成
			} 
		}
	}  				 											 
}   

USART_InitTypeDef USART_InitStructure;
//初始化IO 串口1
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	

void usart1_init(u32 bound)
{  

		NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE); // GPIOA 和 USART1 时钟使能

		USART_DeInit(USART1);                           //复位串口3
   //USART1_TX   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;      //PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);          //初始化PA9
   
    //USART1_RX	  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                          //初始化PA10
	
	USART_InitStructure.USART_BaudRate = bound;                     //波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;          //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;             //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
	USART_Init(USART1, &USART_InitStructure); //初始化串口3
 
	USART_Cmd(USART1, ENABLE);                  //使能串口 
	
	//使能接收中断
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断   
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	TIM3_Init(99,7199);	//10ms中断
	USART1_RX_STA=0;		//清零
	TIM_Cmd(TIM3,DISABLE);	//关闭定时器7
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char *fmt, ...)
{
    u16 i;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)USART1_TX_BUF, fmt, ap);
    va_end(ap);

    i = strlen((const char *)USART1_TX_BUF);

    for (u16 j = 0; j < i; j++)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // 等待发送完毕
        USART_SendData(USART1, USART1_TX_BUF[j]);
    }
}


//串口接收使能控制
//enable:0,关闭 1,打开
void usart1_rx(u8 enable)
{
	 USART_Cmd(USART1, DISABLE); //失能串口 
	
	 if(enable)
	 {
		 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	 }else
	 {
		 USART_InitStructure.USART_Mode = USART_Mode_Tx;//只发送 
	 }
	 
	 USART_Init(USART1, &USART_InitStructure); //初始化串口1
   USART_Cmd(USART1, ENABLE); //使能串口 	
}

