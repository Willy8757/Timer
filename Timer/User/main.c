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
	
	int cnt = 5;
	Set_Cnt(cnt);
	OLED_ShowNum(1, 5, cnt, 5);
	
	while(1){
		
	}

}


