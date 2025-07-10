#include "ch32v30x.h"
#include "TaoJC.h"
#include "./SYSTEM/usart/usart.h"
#include <string.h> 
#include "..\Drivers\BSP\My_macros.h"

#define USART3_REC_LEN 256

uint8_t flag1=1;

uint8_t g_usart3_rx_buf[USART3_REC_LEN];  /* 接收缓冲, 最大USART3_REC_LEN个字节 */
uint16_t g_usart3_rx_sta = 0;             /* 接收状态标记 */

uint8_t receive_state = 0;                /* 接收状态机状态 */
uint8_t receive_index = 0;                /* 当前接收索引 */
uint8_t data_length = 0;                  /* 数据长度 */
uint8_t received_data[256];               /* 接收数据缓冲区 */
uint8_t array_index = 0;                  /* 数组索引 */
char Array[MAX_ELEMENTS] = {0};           /* 数组数据 */
uint16_t received_crc = 0;                /* 接收的CRC校验值 */

/* 串口3初始化函数 */
void USART3_Init(uint32_t bound)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  //开启USART3的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //开启GPIOB的时钟
    
    /* 串口3的GPIO初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //将PB10引脚初始化为复用推挽输出
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //将PB11引脚初始化为上拉输入

    USART_InitTypeDef USART_InitStructure;                  //定义结构体变量
    USART_InitStructure.USART_BaudRate = bound;             //波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //硬件流控制，不需要
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //模式，发送模式和接收模式均选择
    USART_InitStructure.USART_Parity = USART_Parity_No;     //奇偶校验，不需要
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  //停止位，选择1位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //字长，选择8位
    USART_Init(USART3, &USART_InitStructure);               //将结构体变量交给USART_Init，配置USART3

    /* 中断输出配置 */
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //开启串口接收数据的中断
    
    /* NVIC中断分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);         //配置NVIC为分组2

    /* NVIC配置 */
    NVIC_InitTypeDef NVIC_InitStructure;                    //定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;       //选择配置NVIC的USART3线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //指定NVIC线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //指定NVIC线路的抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;      //指定NVIC线路的响应优先级为2
    NVIC_Init(&NVIC_InitStructure);                          //将结构体变量交给NVIC_Init，配置NVIC外设
    
    /* USART使能 */
    USART_Cmd(USART3, ENABLE);                               //使能USART3，串口开始运行
}

/**
 * @brief       串口3中断服务函数
 * @param       无
 * @retval      无
 */
__attribute__((interrupt("WCH-Interrupt-fast")))
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  /* 接收中断 */
    {
        uint8_t received_byte = USART_ReceiveData(USART3);  // 读取数据
        
        /* 检测是否收到 A1 */
        if (received_byte == 0xA1) {
           HMI_send_num("t0.txt",flag1);
           flag1++;
        // UsartPrintf(USART3, "ok\r\n");  // 打印 "ok"
        }
        
        /* 清除中断标志 */
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

void HMI_send_num(char* name, int num)
{
    UsartPrintf(USART3,"%s=\"%d\"\xff\xff\xff",name,num);
}

void HMI_send_string(char* name, char* showdata)
{
    UsartPrintf(USART3,"%s=\"%s\"\xff\xff\xff",name,showdata);
}

// void TaoJC_Send(void)
// {




// }