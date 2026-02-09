#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Buzzer.h"

int16_t cnt = 0;
_Bool counting = 0;

void Key_init(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	
	
	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;       // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;   // PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//Button
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;          
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           
	NVIC_Init(&NVIC_InitStruct);
	
	//Timer
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM2, ENABLE);
}

void Set_Cnt(int16_t i){
	cnt = i;
}

void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET && counting){
		if(cnt >= 0){
			OLED_ShowNum(1, 5, cnt, 5);
		}
		
		if(cnt == 0 || !GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12)){
			Buzzer_turn();
			
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12)) {
                counting = 0;
				Set_Cnt(0);
            }
		}
		cnt--;
		
		
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

}

void EXTI1_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line1) != RESET){
		counting = 1;
		TIM_SetCounter(TIM2, 9999);
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}


uint8_t Key_getnum(void){
	uint8_t key_num = 0;
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)){
		Delay(20);
		while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1));
		Delay(20);
		key_num = 1;
	}
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)){
		Delay(20);
		while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11));
		Delay(20);
		key_num = 2;
	}
	
	return key_num;

}

