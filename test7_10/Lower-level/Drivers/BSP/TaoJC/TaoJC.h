#ifndef _TAOJC_H
#define _TAOJC_H

#define USART3_REC_LEN               200         /* 定义最大接收字节数 200 */
#define FRAME_HEADER         0xFF
#define FRAME_TAIL           0xDD
#define MAX_ELEMENTS 100  //最大分拣量
// extern uint8_t Serial_TxPacket[];
// extern uint8_t Serial_RxPacket[];
void USART3_Init(uint32_t bound);
void ProcessCommand(char* cmd);
void ParseReceivedData(void);
void HMI_send_num(char* name, int num);
void HMI_send_string(char* name, char* showdata);
void TaoJC_Send(void);


#endif 
