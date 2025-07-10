#include "ch32v30x.h"
#include "TaoJC.h"
#include "./SYSTEM/usart/usart.h"
#include <string.h> 
#include "..\Drivers\BSP\My_macros.h"

#define USART3_REC_LEN 256

uint8_t flag1=1;

uint8_t g_usart3_rx_buf[USART3_REC_LEN];  /* ���ջ���, ���USART3_REC_LEN���ֽ� */
uint16_t g_usart3_rx_sta = 0;             /* ����״̬��� */

uint8_t receive_state = 0;                /* ����״̬��״̬ */
uint8_t receive_index = 0;                /* ��ǰ�������� */
uint8_t data_length = 0;                  /* ���ݳ��� */
uint8_t received_data[256];               /* �������ݻ����� */
uint8_t array_index = 0;                  /* �������� */
char Array[MAX_ELEMENTS] = {0};           /* �������� */
uint16_t received_crc = 0;                /* ���յ�CRCУ��ֵ */

/* ����3��ʼ������ */
void USART3_Init(uint32_t bound)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  //����USART3��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //����GPIOB��ʱ��
    
    /* ����3��GPIO��ʼ�� */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //��PB10���ų�ʼ��Ϊ�����������
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //��PB11���ų�ʼ��Ϊ��������

    USART_InitTypeDef USART_InitStructure;                  //����ṹ�����
    USART_InitStructure.USART_BaudRate = bound;             //������
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ�������ƣ�����Ҫ
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //ģʽ������ģʽ�ͽ���ģʽ��ѡ��
    USART_InitStructure.USART_Parity = USART_Parity_No;     //��żУ�飬����Ҫ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  //ֹͣλ��ѡ��1λ
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�ֳ���ѡ��8λ
    USART_Init(USART3, &USART_InitStructure);               //���ṹ���������USART_Init������USART3

    /* �ж�������� */
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //�������ڽ������ݵ��ж�
    
    /* NVIC�жϷ��� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);         //����NVICΪ����2

    /* NVIC���� */
    NVIC_InitTypeDef NVIC_InitStructure;                    //����ṹ�����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;       //ѡ������NVIC��USART3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //ָ��NVIC��·ʹ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //ָ��NVIC��·����ռ���ȼ�Ϊ2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;      //ָ��NVIC��·����Ӧ���ȼ�Ϊ2
    NVIC_Init(&NVIC_InitStructure);                          //���ṹ���������NVIC_Init������NVIC����
    
    /* USARTʹ�� */
    USART_Cmd(USART3, ENABLE);                               //ʹ��USART3�����ڿ�ʼ����
}

/**
 * @brief       ����3�жϷ�����
 * @param       ��
 * @retval      ��
 */
__attribute__((interrupt("WCH-Interrupt-fast")))
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  /* �����ж� */
    {
        uint8_t received_byte = USART_ReceiveData(USART3);  // ��ȡ����
        
        /* ����Ƿ��յ� A1 */
        if (received_byte == 0xA1) {
           HMI_send_num("t0.txt",flag1);
           flag1++;
        // UsartPrintf(USART3, "ok\r\n");  // ��ӡ "ok"
        }
        
        /* ����жϱ�־ */
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