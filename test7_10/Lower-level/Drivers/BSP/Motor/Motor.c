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
  * 函    数：直流电机设置速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_4);	//PD4置高电平
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);	//PD3置低电平，设置方向为正转
		PWM_SetCompare1(Speed);				//PWM设置为速度值
	}
	else									//否则，即设置反转的速度值
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_4);	//PD4置低电平
		GPIO_SetBits(GPIOD, GPIO_Pin_3);	//PD3置高电平，设置方向为反转
		PWM_SetCompare1(-Speed);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}
}
