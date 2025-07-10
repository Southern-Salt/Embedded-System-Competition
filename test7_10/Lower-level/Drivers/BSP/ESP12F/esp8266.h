#ifndef _ESP8266_H_
#define _ESP8266_H_


#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

#define SEND_OK 0
#define SEND_ERROR 1


unsigned char *ESP8266_GetIPD(unsigned short timeOut);

void ESP8266_GotIP(void);
void ESP8266_Init(void);

void ESP8266_Clear(void);

void ESP8266_SendData(unsigned char *data, unsigned short len);

void ESP8266_SmartConfig(void);

_Bool ESP8266_WaitRecive(void);

_Bool ESP8266_SendCmd(char *cmd, char *res);





#endif
