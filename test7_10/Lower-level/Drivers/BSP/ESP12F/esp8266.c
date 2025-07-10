//网络设备驱动
#include "esp8266.h"

//硬件驱动
#include "..\Drivers\SYSTEM\delay\delay.h"
#include "..\Drivers\SYSTEM\usart\usart.h"

//C库
#include <string.h>
#include <stdio.h>

#include "..\Drivers\BSP\My_macros.h"


// WiFi账号和密码全局变量
char ESP8266_WIFI_ACCOUNT[32] = "8B117";     // 默认WiFi账号
char ESP8266_WIFI_PASSWORD[32] = "11223344"; // 默认WiFi密码

#define ESP8266_WIFI_INFO    "AT+CWJAP=\"%s\",\"%s\"\r\n"  // 使用变量的WiFi连接命令
#define ESP8266_ONENET_INFO  "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"
 
#define MAX_SENTENCE_LENGTH 128  // 定义最大语句长度

uint8_t sentence_buf[MAX_SENTENCE_LENGTH];  // 语句缓冲区
uint16_t sentence_len = 0;                  // 当前语句长度
uint8_t sentence_ready = 0;                 // 语句完成标志

unsigned char esp8266_buf[512];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;


//==========================================================
//	函数名称：	ESP8266_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	函数名称：	ESP8266_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		esp8266_cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	esp8266_cntPre = esp8266_cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志

}

//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut =200;

	Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
			{
				ESP8266_Clear();									//清空缓存
				
				return 0;
			}
		}
		
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[64];
	
	ESP8266_Clear();								//清空接收缓存
	
	//先发送要发送数据的指令做准备
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
	{
		//既然准备完毕即可开始发送数据
		Usart_SendString(USART2, data, len);		//发送设备连接请求数据
	}

}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针

}


//==========================================================
//	函数名称：	ESP8266_GotIP
//
//	函数功能：	初始化ESP8266
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		PA2是USART2_TX，PA3是USART2_RX
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
    
    // 使用全局变量中的WiFi账号和密码
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
//	函数名称：	ESP8266_Init
//
//	函数功能：	初始化ESP8266
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		PA2是USART2_TX，PA3是USART2_RX
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
    printf("接入onenet成功\r\n");

    /*数据初始化*/
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
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
    {
        uint8_t ch = USART2->DATAR;  // 读取接收到的字符
        
        // 存储到ESP8266缓冲区（保持原有功能）
        if(esp8266_cnt >= sizeof(esp8266_buf))    
            esp8266_cnt = 0;         // 防止缓冲区溢出
        esp8266_buf[esp8266_cnt++] = ch;
        
      
        
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);  //清除标志位
    }
}