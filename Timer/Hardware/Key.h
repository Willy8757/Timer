#include "stm32f10x.h"                  // Device header

#ifndef __KEY_H
#define __KEY_H

void Key_init(void);
void Set_Cnt(int16_t);
uint8_t Key_getnum(void);


#endif
