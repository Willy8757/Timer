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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;       // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;   // PSC : 0.1ms
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM2, ENABLE);
}

void Set_Cnt(int16_t i){
	cnt = i;
}

void TIM2_IRQHandler(void){
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)){
		Delay(20);
		while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1));
		Delay(20);
		counting = 1;
	}
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET && counting){
		if(cnt >= 0){
			OLED_Clear();
			OLED_ShowNum(1, 5, cnt, 5);
		}
		
		if(cnt == 0){
			Buzzer_on();
			Delay(100);
			Buzzer_off();
		}
		cnt-=1;
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

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

