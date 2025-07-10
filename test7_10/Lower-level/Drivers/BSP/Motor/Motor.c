#include "ch32v30x.h"
#include "..\Drivers\BSP\Motor\PWM.h"
#include "..\Drivers\BSP\Motor\Motor.h"

void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	PWM_Init(100,14400);


}

/**
  * ��    ����ֱ����������ٶ�
  * ��    ����Speed Ҫ���õ��ٶȣ���Χ��-100~100
  * �� �� ֵ����
  */
void Motor_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)							//���������ת���ٶ�ֵ
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_4);	//PD4�øߵ�ƽ
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);	//PD3�õ͵�ƽ�����÷���Ϊ��ת
		PWM_SetCompare1(Speed);				//PWM����Ϊ�ٶ�ֵ
	}
	else									//���򣬼����÷�ת���ٶ�ֵ
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_4);	//PD4�õ͵�ƽ
		GPIO_SetBits(GPIOD, GPIO_Pin_3);	//PD3�øߵ�ƽ�����÷���Ϊ��ת
		PWM_SetCompare1(-Speed);			//PWM����Ϊ�����ٶ�ֵ����Ϊ��ʱ�ٶ�ֵΪ��������PWMֻ�ܸ�����
	}
}
