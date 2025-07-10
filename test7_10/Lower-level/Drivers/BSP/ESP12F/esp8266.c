//�����豸����
#include "esp8266.h"

//Ӳ������
#include "..\Drivers\SYSTEM\delay\delay.h"
#include "..\Drivers\SYSTEM\usart\usart.h"

//C��
#include <string.h>
#include <stdio.h>

#include "..\Drivers\BSP\My_macros.h"


// WiFi�˺ź�����ȫ�ֱ���
char ESP8266_WIFI_ACCOUNT[32] = "8B117";     // Ĭ��WiFi�˺�
char ESP8266_WIFI_PASSWORD[32] = "11223344"; // Ĭ��WiFi����

#define ESP8266_WIFI_INFO    "AT+CWJAP=\"%s\",\"%s\"\r\n"  // ʹ�ñ�����WiFi��������
#define ESP8266_ONENET_INFO  "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"
 
#define MAX_SENTENCE_LENGTH 128  // ���������䳤��

uint8_t sentence_buf[MAX_SENTENCE_LENGTH];  // ��仺����
uint16_t sentence_len = 0;                  // ��ǰ��䳤��
uint8_t sentence_ready = 0;                 // �����ɱ�־

unsigned char esp8266_buf[512];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;


//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	�������ƣ�	ESP8266_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		esp8266_cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut =200;

	Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
			{
				ESP8266_Clear();									//��ջ���
				
				return 0;
			}
		}
		
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[64];
	
	ESP8266_Clear();								//��ս��ջ���
	
	//�ȷ���Ҫ�������ݵ�ָ����׼��
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		//��Ȼ׼����ϼ��ɿ�ʼ��������
		Usart_SendString(USART2, data, len);		//�����豸������������
	}

}

//==========================================================
//	�������ƣ�	ESP8266_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//����������
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//��ʱ�ȴ�
	} while(timeOut--);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}


//==========================================================
//	�������ƣ�	ESP8266_GotIP
//
//	�������ܣ�	��ʼ��ESP8266
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		PA2��USART2_TX��PA3��USART2_RX
//==========================================================
void ESP8266_GotIP(void)
{
    UsartPrintf(USART2,"\r\n");
    UsartPrintf(USART2,"AT\r\n");
    UsartPrintf(USART2,"AT+RST\r\n");
    delay_ms(2000);
    
    UsartPrintf(USART2, "AT\r\n");

    delay_ms(500);
    
    // UsartPrintf(USART2, "AT+CWMODE=1\r\n");
    while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
        delay_ms(1000);
    
    // UsartPrintf(USART2, "AT+CWDHCP=1,1\r\n");
    while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
        delay_ms(1000);
    
    // ʹ��ȫ�ֱ����е�WiFi�˺ź�����
    char wifi_cmd[64];
    sprintf(wifi_cmd, ESP8266_WIFI_INFO, ESP8266_WIFI_ACCOUNT, ESP8266_WIFI_PASSWORD);
    while(ESP8266_SendCmd(wifi_cmd, "GOT IP"))
        delay_ms(3000);
    
    // UsartPrintf(USART2, "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n");
    
    // UsartPrintf(USART1, "5. CIPSTART\r\n");
    while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
        delay_ms(500);
    
    // UsartPrintf(USART1, "6. ESP8266 Init OK\r\n");
    UsartPrintf(USART2,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",DEVICE_NAME,PROID,TOKEN);
    delay_ms(3000);
    UsartPrintf(USART2,"AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n");
    delay_ms(1000);
    UsartPrintf(USART2,"AT+MQTTSUB=0,\"$sys/%s/%s/thing/property/post/reply\",1\r\n",PROID,DEVICE_NAME);
    delay_ms(1000);

    
    UsartPrintf(USART2,"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Res\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",PROID,DEVICE_NAME,0);
    delay_ms(500);
    UsartPrintf(USART2,"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Cap\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",PROID,DEVICE_NAME,0);
    delay_ms(500);
    UsartPrintf(USART2,"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Ind\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",PROID,DEVICE_NAME,0);
}


//==========================================================
//	�������ƣ�	ESP8266_Init
//
//	�������ܣ�	��ʼ��ESP8266
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		PA2��USART2_TX��PA3��USART2_RX
//==========================================================
void ESP8266_Init(void)
{
    ESP8266_Clear();
    UsartPrintf(USART2,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",DEVICE_NAME,PROID,TOKEN);
	delay_ms(1000);
    UsartPrintf(USART2,"AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n");
	delay_ms(1000);
	UsartPrintf(USART2,"AT+MQTTSUB=0,\"$sys/%s/%s/thing/property/post/reply\",1\r\n",PROID,DEVICE_NAME);
	delay_ms(1000);
    printf("����onenet�ɹ�\r\n");

    /*���ݳ�ʼ��*/
	UsartPrintf(USART2,"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Res\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",PROID,DEVICE_NAME,0);
    UsartPrintf(USART2,"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Cap\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",PROID,DEVICE_NAME,0);
    UsartPrintf(USART2,"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Ind\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",PROID,DEVICE_NAME,0);

}




void ESP8266_SmartConfig(void)
{
	UsartPrintf(USART1,"AT+RST\r\n");
	delay_ms(2000);
	
	UsartPrintf(USART1, "1. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"))
		delay_ms(500);
	
	UsartPrintf(USART1, "2. CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		delay_ms(500);
	
	UsartPrintf(USART1, "3. AT+CWDHCP\r\n");
	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
		delay_ms(500);

	UsartPrintf(USART1, "4. AT+CWSTARTSMART=3\r\n");
	while(ESP8266_SendCmd("AT+CWSTARTSMART=3\r\n", "AIRKISS"))
		delay_ms(500);
	// UsartPrintf(USART1, "5. CIPSTART\r\n");
	// while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
	// 	delay_ms(500);
	
	UsartPrintf(USART1, "5. ESP8266 Init OK\r\n");
    UsartPrintf(USART2,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",DEVICE_NAME,PROID,TOKEN);
	delay_ms(1000);
    UsartPrintf(USART2,"AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n");
	delay_ms(1000);
	UsartPrintf(USART2,"AT+MQTTSUB=0,\"$sys/%s/%s/thing/property/post/reply\",1\r\n",PROID,DEVICE_NAME);
	delay_ms(1000);
}


__attribute__((interrupt("WCH-Interrupt-fast")))
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�����ж�
    {
        uint8_t ch = USART2->DATAR;  // ��ȡ���յ����ַ�
        
        // �洢��ESP8266������������ԭ�й��ܣ�
        if(esp8266_cnt >= sizeof(esp8266_buf))    
            esp8266_cnt = 0;         // ��ֹ���������
        esp8266_buf[esp8266_cnt++] = ch;
        
      
        
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);  //�����־λ
    }
}