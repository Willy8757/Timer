#include "stm32f10x.h"                  // Device header


#ifndef __OLED_H
#define __OLED_H


void OLED_I2C_start(void);
void OLED_I2C_stop(void);
void OLED_SendBytes(uint8_t Byt);
void OLED_WriteData(uint8_t Data);
void OLED_WriteCommand(uint8_t Command);
void OLED_SetCursor(uint8_t Page, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, char c);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_Clear(void);
void OLED_Init(void);
#endif
