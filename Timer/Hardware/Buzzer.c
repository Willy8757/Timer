#include "stm32f10x.h"                  // Device header


void Buzzer_init(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void Buzzer_on(void){
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

void Buzzer_off(void){
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

void Buzzer_turn(void){
	if(!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12))
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	else GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

