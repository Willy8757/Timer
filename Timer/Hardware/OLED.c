#include "stm32f10x.h"
#include "OLED_Font.h"
#include "Delay.h"
#include "Buzzer.h"

#define OLED_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))


void OLED_I2C_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //Open drain
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_SCL(1);
	OLED_SDA(1);
}



void OLED_I2C_start(){
	
	OLED_SDA(1); //init
	OLED_SCL(1);
	OLED_SDA(0); //start : SDA go down when SCL = 1
	OLED_SCL(0); //transmit when SCL = 1

}

void OLED_I2C_stop(){
	
	OLED_SDA(0);
	OLED_SCL(1);
	OLED_SDA(1); //stop

}


void OLED_SendBytes(uint8_t Byt){
	for(uint8_t i = 0; i < 8; i++){
		OLED_SDA(Byt & (0x80 >> i)); //send hightest bit
		Delay_us(5);
		OLED_SCL(1);
		Delay_us(5);
		OLED_SCL(0);
		Delay_us(5);
	}
	
	OLED_SDA(1);
	OLED_SCL(1);
	/*ack*/
	Delay_us(5);
	OLED_SCL(0);

}

void OLED_WriteData(uint8_t Data){

	OLED_I2C_start();
	OLED_SendBytes(0x78); /*address of SSD1306 is 0x3C, but the last bit in I2C denote receive of send data, so 0x3C << 1 = 0011 1100 << 1 = 0111 1000
							and set last bit as 0(write data) -> 0x78*/
	OLED_SendBytes(0x40); //0x40 : send data, 0x00 : send command
	OLED_SendBytes(Data);
	OLED_I2C_stop();

}

void OLED_WriteCommand(uint8_t Command){
	
	OLED_I2C_start();
	OLED_SendBytes(0x78);
	OLED_SendBytes(0x00); //command
	OLED_SendBytes(Command);
	OLED_I2C_stop();

}

void OLED_SetCursor(uint8_t Page, uint8_t y){
	
	OLED_WriteCommand(0xB0 | Page); //0xB? : command to set page
	OLED_WriteCommand(0x10 | ((y & 0xF0) >> 4)); //0x1? : command to set 4-bit high
	OLED_WriteCommand(0x00 | (y & 0x0F)); //0x0?: command to set 4-bit low
}



void OLED_ShowChar(uint8_t x, uint8_t y, char c){
	OLED_SetCursor((x - 1) * 2 , (y - 1) * 8); /*size of char on OLED is 16*8, but only 8 pixel each page and 8 pixel each row
												 so first char use page0, 1, the next one use page2, 3, ...etc	*/
	for(uint8_t i = 0; i < 8; i++){
		OLED_WriteData(OLED_F8x16[c - ' '][i]);
	}
	
	OLED_SetCursor((x - 1) * 2 + 1, ((y - 1) * 8));
	
	for (uint8_t i = 0; i < 8; i++){
		OLED_WriteData(OLED_F8x16[c - ' '][i + 8]);		
	}

}

uint32_t OLED_Pow(uint32_t X, uint32_t Y){
	uint32_t Result = 1;
	while (Y--){
		Result *= X;
	}
	return Result;
}


void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length){
	uint8_t i;
	for (i = 0; i < Length; i++){
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}



void OLED_Clear(){
	for(uint8_t i = 0; i < 8; i++){
		OLED_SetCursor(i, 0);
		for(uint8_t j = 0; j < 128; j++){
			OLED_WriteData(0x00);
		}
	}
}


void OLED_Init(){
	
	Delay_us(10);
	OLED_I2C_Init();
	
	OLED_WriteCommand(0XAE); //Display off
	
	OLED_WriteCommand(0X8D); 
	OLED_WriteCommand(0X14); //Charge Pump
	
	OLED_WriteCommand(0xA1); //from left
	OLED_WriteCommand(0xC8); //from top
	
	OLED_WriteCommand(0XAF); //Display on
	OLED_Clear();

}









