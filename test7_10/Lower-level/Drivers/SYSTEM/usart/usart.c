#include "./SYSTEM/usart/usart.h"
#include <stdarg.h>
#include "..\Drivers\BSP\ESP12F\esp8266.h"
#include <stdio.h>

#if USART_EN_RX                         /* 如果使能了接收 */

void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

uint8_t g_usart_rx_buf[USART_REC_LEN];  /* 接收缓冲, 最大USART_REC_LEN个字节 */
uint8_t receive1_state = 0;                /* 接收状态机状态 */
uint8_t receive1_index = 0;                /* 当前接收索引 */
uint8_t received1_data[256];               /* 接收数据缓冲区 */

/*  接收状态
 *  bit15，      接收完成标志
 *  bit14，      接收到0x0d
 *  bit13~0，    接收到的有效字节数目
*/
uint16_t g_usart_rx_sta = 0;

#endif

/**
 * @brief       串口1初始化函数
 * @param       无
 * @retval      无
 */
void usart1_init(uint32_t bound)
{
    GPIO_InitTypeDef  gpio_init_struct;
    USART_InitTypeDef uartx_init_struct;
    NVIC_InitTypeDef  nvic_init_struct;

    USART_TX_GPIO_CLK_ENABLE();                              /* 使能串口TX脚时钟 */
    USART_RX_GPIO_CLK_ENABLE();                              /* 使能串口RX脚时钟 */
    USART_UX_CLK_ENABLE();                                   /* 使能串口时钟 */
    
   


   /*串口1的GPIO初始化*/
    gpio_init_struct.GPIO_Pin = USART_TX_GPIO_PIN;           /* 串口发送引脚号 */
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;            /* 复用推挽输出 */
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;          /* IO速度设置为50M */
    GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = USART_RX_GPIO_PIN;           /* 串口RX脚 模式设置 */
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;      /* 浮空输入 */
    GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);        /* 串口RX脚,设置成输入模式 */

    
	


    /* USART1初始化设置*/
    uartx_init_struct.USART_BaudRate = bound;                                       /* 波特率 */
    uartx_init_struct.USART_WordLength = USART_WordLength_8b;                       /* 字长为8位数据格式 */
    uartx_init_struct.USART_StopBits = USART_StopBits_1;                            /* 一个停止位 */
    uartx_init_struct.USART_Parity = USART_Parity_No;                               /* 无奇偶校验位 */
    uartx_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   /* 无硬件流控 */
    uartx_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   /* 收发模式 */
    USART_Init(USART_UX,&uartx_init_struct);                                        /* 初始化串口 */

#if USART_EN_RX
    nvic_init_struct.NVIC_IRQChannel = USART_UX_IRQn;        /* 使能USART中断通道 */
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority=1 ;   /* 抢占优先级3 */
    nvic_init_struct.NVIC_IRQChannelSubPriority = 1;         /* 响应优先级3 */
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;            /* IRQ通道使能  */
    NVIC_Init(&nvic_init_struct);                            /* 初始化NVIC */

    USART_ITConfig(USART_UX, USART_IT_RXNE, ENABLE);         /* 开启串口接受中断 */
#endif
    USART_Cmd(USART_UX, ENABLE);                             /* 使能串口 */
}

#if USART_EN_RX                                              /* 如果使能了接收 */





void USART2_Init(uint32_t bound)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  //开启USART2的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
    
    
    /*串口2的GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA2引脚初始化为复用推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA3引脚初始化为上拉输入

    USART_InitTypeDef USART_InitStructure;					//定义结构体变量
	USART_InitStructure.USART_BaudRate =bound;				//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
	USART_Init(USART2, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART2
	

    	/*中断输出配置*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//配置NVIC为分组1

    /*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//选择配置NVIC的USART2线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;		//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*USART使能*/
	USART_Cmd(USART2, ENABLE);								//使能USART2，串口开始运行
        

}

// void USART2_Init(uint32_t bound)
// {

//      GPIO_InitTypeDef   GPIO_InitStructure={0};

//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

//     /* USART2 TX-->A.2   RX-->A.3 */
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //设置PA2为复用推挽输出
//     GPIO_Init(GPIOA, &GPIO_InitStructure);
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //设置PA3为浮空输入
//     GPIO_Init(GPIOA, &GPIO_InitStructure);

//     USART_InitTypeDef  USART_InitStructure={0};
  
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

//     USART_InitStructure.USART_BaudRate = bound;
//     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//     USART_InitStructure.USART_StopBits = USART_StopBits_1;
//     USART_InitStructure.USART_Parity = USART_Parity_No;
//     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//     USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
//     USART_Init(USART2, &USART_InitStructure);

//         NVIC_InitTypeDef   NVIC_InitStructure={0};

//     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;      //抢占优先级为1
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           //子优先级为1
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQ通道使能
//     NVIC_Init(&NVIC_InitStructure);                              //中断优先级初始化

//     USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
//     USART_Cmd(USART2,ENABLE);
// }


/**
 * @brief       串口1中断服务函数
 * @param       无
 * @retval      无
 */
__attribute__((interrupt("WCH-Interrupt-fast")))
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  /* 如果是接收中断 */
    {
        uint8_t received1_byte = USART_ReceiveData(USART1);  // 读取接收到的数据
        
        /* 状态机方式处理数据帧 */
        switch(receive1_state)
        {
            case 0:  // 等待帧头0xFF
                if(received1_byte == 0xFF)
                {
                    received1_data[0] = received1_byte;
                    receive1_index = 1;
                    receive1_state = 1;  // 进入接收命令码状态
                }
                break;
                
            case 1:  // 接收命令码(01/02/03)
                if(received1_byte == 0x01 || received1_byte == 0x02 || received1_byte == 0x03)
                {
                    received1_data[1] = received1_byte;
                    receive1_index = 2;
                    receive1_state = 2;  // 进入接收帧尾状态
                }
                else
                {
                    receive1_state = 0;  // 不是有效命令码，重置
                }
                break;
                
            case 2:  // 接收帧尾0xDD
                if(received1_byte == 0xDD)
                {
                    received1_data[2] = received1_byte;
                    receive1_index = 3;
                    
                    /* 帧校验成功，处理数据 */
                    UsartPrintf(USART1, "Valid Frame Received: ");
                    for(int i = 0; i < receive1_index; i++)
                    {
                        UsartPrintf(USART1, "%02X ", received1_data[i]);
                    }
                    UsartPrintf(USART1, "\r\n");
                    
                    /* 根据不同命令码执行不同操作 */
                    switch(received1_data[1])
                    {
                        case 0x01:
                            UsartPrintf(USART1, "Command 01 Detected\r\n");
                            // 执行命令01对应的操作
                            break;
                            
                        case 0x02:
                            UsartPrintf(USART1, "Command 02 Detected\r\n");
                            // 执行命令02对应的操作
                            break;
                            
                        case 0x03:
                            UsartPrintf(USART1, "Command 03 Detected\r\n");
                            // 执行命令03对应的操作
                            break;
                    }
                }
                else
                {
                    UsartPrintf(USART1, "Frame Error: Invalid Tail\r\n");
                }
                
                receive1_state = 0;  // 无论帧尾是否正确，都重置状态机
                break;
                
            default:
                receive1_state = 0;  // 错误状态，重置
                break;
        }
        
        /* 必须清除中断标志 */
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
#endif

/**
 * @brief       _write函数
 * @param       *buf : 要发送的数据,size: 要发送的数据长度
 * @retval      size : 数据长度
 */
int _write(int fd, char *buf, int size)
{
    int i;

    for(i=0; i<size; i++)
    {
        while (USART_GetFlagStatus(USART_UX, USART_FLAG_TC) == RESET);
        USART_SendData(USART_UX, *buf++);
    }
    return size;
}



/**************************实现函数********************************************
函数说明：发送一个字节

*******************************************************************************/ 
void Usart_SendByte(USART_TypeDef *USARTx,uint8_t Byte)
{

    USART_SendData(USARTx,Byte);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}



/**************************实现函数********************************************
函数说明：发送数组

*******************************************************************************/ 
void Usart_SendArray(USART_TypeDef *USARTx,uint8_t *Array, uint16_t Length)
{
    uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Usart_SendByte(USARTx,*Array);		//依次调用Serial_SendByte发送每个字节数据
	}
}



/**************************实现函数********************************************
函数说明：发送字符串

*******************************************************************************/ 
void Usart_SendStr(USART_TypeDef *USARTx, char *str)
{
    uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Usart_SendByte(USARTx,str[i]);
	}

}

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}


/*
****************************
* 函 数 名：fputc
* 功能说明：重定义putc函数，这样可以使用printf函数从串口1打印输出
* 形    参：无
* 返 回 值：无
****************************
*/
int fputc(int ch,FILE *f)
{
    USART_ClearFlag(USART1,USART_FLAG_TC);
   USART_SendData(USART1,(uint8_t) ch);
   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
   return (ch);
}




/*
************************************************************
*	函数名称：	UsartPrintf
*
*	函数功能：	格式化打印信息，便于调试
*
*	入口参数：	USARTx：串口组
*				fmt：不定长参
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{

	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
	va_end(ap);
	
	while(*pStr != 0)
	{   
        USART_ClearFlag(USARTx,USART_FLAG_TC);
		USART_SendData(USARTx, *pStr++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}

}