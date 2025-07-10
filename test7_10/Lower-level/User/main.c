/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-07-20
 * @brief       串口通信 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 CH32V307开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com/forum.php
 * 公司网址:www.alientek.com
 * 购买地址:zhengdianyuanzi.tmall.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "..\Drivers\BSP\ESP12F\esp8266.h"
#include "..\Drivers\BSP\My_macros.h"
#include "..\Drivers\BSP\TaoJC\TaoJC.h"
#include "..\Drivers\BSP\Motor\PWM.h"
#include "..\Drivers\BSP\Motor\Motor.h"



int main(void)
{
    // uint16_t t;
    // uint8_t len;
    // uint16_t times = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     /* 中断优先级分组2 */
    delay_init(144);                                    /* 延时初始化 */
    usart1_init(115200);                                 /* 串口初始化为115200 */
    
    USART2_Init(115200);
    USART3_Init(115200);
    led_init();                                         /* 初始化LED */
  //ESP8266_GotIP();					//初始化ESP8266


// delay_s(3);
// UsartPrintf(USART2,"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Res\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",PROID,DEVICE_NAME,6);
//     delay_ms(500);
// 	UsartPrintf(USART2,"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Cap\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",PROID,DEVICE_NAME,7);
//     delay_ms(500);
// 	UsartPrintf(USART2,"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Ind\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",PROID,DEVICE_NAME,8);
    
    // UsartPrintf(USART2,"OK\r\n");
   Motor_Init();
   Motor_SetSpeed(40);
  //  delay_s(3);
  //  Motor_SetSpeed(0);
    while(1)
    {

    }
}