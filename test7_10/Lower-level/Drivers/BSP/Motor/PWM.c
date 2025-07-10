#include "ch32v30x.h"
#include "..\Drivers\BSP\Motor\PWM.h"


void PWM_Init(u16 arr,u16 psc)
{
GPIO_InitTypeDef GPIO_InitStructure={0};
	TIM_OCInitTypeDef TIM_OCInitStructure={0};
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	TIM_TimeBaseInitStructure.TIM_Period = arr-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);


  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init( TIM1, &TIM_OCInitStructure );

	TIM_CtrlPWMOutputs(TIM1, ENABLE );
	TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );
	TIM_ARRPreloadConfig( TIM1, ENABLE );
	TIM_Cmd( TIM1, ENABLE );


}


/*≈‰÷√ccr*/
void PWM_SetCompare1(uint16_t Compare)
{
   
    TIM_SetCompare1(TIM1,Compare);
}