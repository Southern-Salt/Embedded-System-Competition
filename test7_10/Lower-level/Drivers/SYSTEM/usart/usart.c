#include "./SYSTEM/usart/usart.h"
#include <stdarg.h>
#include "..\Drivers\BSP\ESP12F\esp8266.h"
#include <stdio.h>

#if USART_EN_RX                         /* ���ʹ���˽��� */

void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

uint8_t g_usart_rx_buf[USART_REC_LEN];  /* ���ջ���, ���USART_REC_LEN���ֽ� */
uint8_t receive1_state = 0;                /* ����״̬��״̬ */
uint8_t receive1_index = 0;                /* ��ǰ�������� */
uint8_t received1_data[256];               /* �������ݻ����� */

/*  ����״̬
 *  bit15��      ������ɱ�־
 *  bit14��      ���յ�0x0d
 *  bit13~0��    ���յ�����Ч�ֽ���Ŀ
*/
uint16_t g_usart_rx_sta = 0;

#endif

/**
 * @brief       ����1��ʼ������
 * @param       ��
 * @retval      ��
 */
void usart1_init(uint32_t bound)
{
    GPIO_InitTypeDef  gpio_init_struct;
    USART_InitTypeDef uartx_init_struct;
    NVIC_InitTypeDef  nvic_init_struct;

    USART_TX_GPIO_CLK_ENABLE();                              /* ʹ�ܴ���TX��ʱ�� */
    USART_RX_GPIO_CLK_ENABLE();                              /* ʹ�ܴ���RX��ʱ�� */
    USART_UX_CLK_ENABLE();                                   /* ʹ�ܴ���ʱ�� */
    
   


   /*����1��GPIO��ʼ��*/
    gpio_init_struct.GPIO_Pin = USART_TX_GPIO_PIN;           /* ���ڷ������ź� */
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;            /* ����������� */
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;          /* IO�ٶ�����Ϊ50M */
    GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin = USART_RX_GPIO_PIN;           /* ����RX�� ģʽ���� */
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;      /* �������� */
    GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);        /* ����RX��,���ó�����ģʽ */

    
	


    /* USART1��ʼ������*/
    uartx_init_struct.USART_BaudRate = bound;                                       /* ������ */
    uartx_init_struct.USART_WordLength = USART_WordLength_8b;                       /* �ֳ�Ϊ8λ���ݸ�ʽ */
    uartx_init_struct.USART_StopBits = USART_StopBits_1;                            /* һ��ֹͣλ */
    uartx_init_struct.USART_Parity = USART_Parity_No;                               /* ����żУ��λ */
    uartx_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   /* ��Ӳ������ */
    uartx_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   /* �շ�ģʽ */
    USART_Init(USART_UX,&uartx_init_struct);                                        /* ��ʼ������ */

#if USART_EN_RX
    nvic_init_struct.NVIC_IRQChannel = USART_UX_IRQn;        /* ʹ��USART�ж�ͨ�� */
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority=1 ;   /* ��ռ���ȼ�3 */
    nvic_init_struct.NVIC_IRQChannelSubPriority = 1;         /* ��Ӧ���ȼ�3 */
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;            /* IRQͨ��ʹ��  */
    NVIC_Init(&nvic_init_struct);                            /* ��ʼ��NVIC */

    USART_ITConfig(USART_UX, USART_IT_RXNE, ENABLE);         /* �������ڽ����ж� */
#endif
    USART_Cmd(USART_UX, ENABLE);                             /* ʹ�ܴ��� */
}

#if USART_EN_RX                                              /* ���ʹ���˽��� */





void USART2_Init(uint32_t bound)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  //����USART2��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
    
    
    /*����2��GPIO��ʼ��*/
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA2���ų�ʼ��Ϊ�����������
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA3���ų�ʼ��Ϊ��������

    USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate =bound;				//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(USART2, &USART_InitStructure);				//���ṹ���������USART_Init������USART2
	

    	/*�ж��������*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//����NVICΪ����1

    /*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//ѡ������NVIC��USART2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART2, ENABLE);								//ʹ��USART2�����ڿ�ʼ����
        

}

// void USART2_Init(uint32_t bound)
// {

//      GPIO_InitTypeDef   GPIO_InitStructure={0};

//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

//     /* USART2 TX-->A.2   RX-->A.3 */
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //����PA2Ϊ�����������
//     GPIO_Init(GPIOA, &GPIO_InitStructure);
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //����PA3Ϊ��������
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
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;      //��ռ���ȼ�Ϊ1
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           //�����ȼ�Ϊ1
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQͨ��ʹ��
//     NVIC_Init(&NVIC_InitStructure);                              //�ж����ȼ���ʼ��

//     USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
//     USART_Cmd(USART2,ENABLE);
// }


/**
 * @brief       ����1�жϷ�����
 * @param       ��
 * @retval      ��
 */
__attribute__((interrupt("WCH-Interrupt-fast")))
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  /* ����ǽ����ж� */
    {
        uint8_t received1_byte = USART_ReceiveData(USART1);  // ��ȡ���յ�������
        
        /* ״̬����ʽ��������֡ */
        switch(receive1_state)
        {
            case 0:  // �ȴ�֡ͷ0xFF
                if(received1_byte == 0xFF)
                {
                    received1_data[0] = received1_byte;
                    receive1_index = 1;
                    receive1_state = 1;  // �������������״̬
                }
                break;
                
            case 1:  // ����������(01/02/03)
                if(received1_byte == 0x01 || received1_byte == 0x02 || received1_byte == 0x03)
                {
                    received1_data[1] = received1_byte;
                    receive1_index = 2;
                    receive1_state = 2;  // �������֡β״̬
                }
                else
                {
                    receive1_state = 0;  // ������Ч�����룬����
                }
                break;
                
            case 2:  // ����֡β0xDD
                if(received1_byte == 0xDD)
                {
                    received1_data[2] = received1_byte;
                    receive1_index = 3;
                    
                    /* ֡У��ɹ����������� */
                    UsartPrintf(USART1, "Valid Frame Received: ");
                    for(int i = 0; i < receive1_index; i++)
                    {
                        UsartPrintf(USART1, "%02X ", received1_data[i]);
                    }
                    UsartPrintf(USART1, "\r\n");
                    
                    /* ���ݲ�ͬ������ִ�в�ͬ���� */
                    switch(received1_data[1])
                    {
                        case 0x01:
                            UsartPrintf(USART1, "Command 01 Detected\r\n");
                            // ִ������01��Ӧ�Ĳ���
                            break;
                            
                        case 0x02:
                            UsartPrintf(USART1, "Command 02 Detected\r\n");
                            // ִ������02��Ӧ�Ĳ���
                            break;
                            
                        case 0x03:
                            UsartPrintf(USART1, "Command 03 Detected\r\n");
                            // ִ������03��Ӧ�Ĳ���
                            break;
                    }
                }
                else
                {
                    UsartPrintf(USART1, "Frame Error: Invalid Tail\r\n");
                }
                
                receive1_state = 0;  // ����֡β�Ƿ���ȷ��������״̬��
                break;
                
            default:
                receive1_state = 0;  // ����״̬������
                break;
        }
        
        /* ��������жϱ�־ */
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
#endif

/**
 * @brief       _write����
 * @param       *buf : Ҫ���͵�����,size: Ҫ���͵����ݳ���
 * @retval      size : ���ݳ���
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



/**************************ʵ�ֺ���********************************************
����˵��������һ���ֽ�

*******************************************************************************/ 
void Usart_SendByte(USART_TypeDef *USARTx,uint8_t Byte)
{

    USART_SendData(USARTx,Byte);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}



/**************************ʵ�ֺ���********************************************
����˵������������

*******************************************************************************/ 
void Usart_SendArray(USART_TypeDef *USARTx,uint8_t *Array, uint16_t Length)
{
    uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Usart_SendByte(USARTx,*Array);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}



/**************************ʵ�ֺ���********************************************
����˵���������ַ���

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
		USART_SendData(USARTx, *str++);									//��������
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
	}

}


/*
****************************
* �� �� ����fputc
* ����˵�����ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
* ��    �Σ���
* �� �� ֵ����
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
*	�������ƣ�	UsartPrintf
*
*	�������ܣ�	��ʽ����ӡ��Ϣ�����ڵ���
*
*	��ڲ�����	USARTx��������
*				fmt����������
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{

	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//��ʽ��
	va_end(ap);
	
	while(*pStr != 0)
	{   
        USART_ClearFlag(USARTx,USART_FLAG_TC);
		USART_SendData(USARTx, *pStr++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}

}