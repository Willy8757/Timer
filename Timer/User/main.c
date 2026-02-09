#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Buzzer.h"

int main(void){
	Key_init();
	OLED_Init();
	OLED_Clear();
	Buzzer_init();
	
	int cnt = 0;
	OLED_ShowNum(1, 5, 0, 5);
	
	while(1){
		if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)){
			Delay(20);
			while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11));
			Delay(20);
			cnt++;
			Set_Cnt(cnt);
			OLED_ShowNum(1, 5, cnt, 5);
		}
	}

}


