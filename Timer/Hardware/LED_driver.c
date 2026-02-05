#include "stm32f10x.h"                  // Device header


void LED_init(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);

}

void LED_on(uint16_t GPIO_Pin){
	GPIO_ResetBits(GPIOA, GPIO_Pin);
}

void LED_off(uint16_t GPIO_Pin){
	GPIO_SetBits(GPIOA, GPIO_Pin);
}

void LED_turn(uint16_t GPIO_Pin){
	if(!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin))
		GPIO_SetBits(GPIOA, GPIO_Pin);
	else GPIO_ResetBits(GPIOA, GPIO_Pin);
}

