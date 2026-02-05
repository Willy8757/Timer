#include "stm32f10x.h"                  // Device header


void Delay(uint32_t m){
	SysTick->LOAD = (SystemCoreClock / 1000) * m;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x05; //set bit 0, 2 = 1
	
	while((SysTick->CTRL & (1<<16)) == 0); //counterflag on bit 16
	SysTick->CTRL = 0;
}


void Delay_us(uint32_t u){
	SysTick->LOAD = (SystemCoreClock / 1000000) * u;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x05; //set bit 0, 2 = 1
	
	while((SysTick->CTRL & (1<<16)) == 0); //counterflag on bit 16
	SysTick->CTRL = 0;
}

