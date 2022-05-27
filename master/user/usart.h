#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>


void USART1_Config(void);
void USART3_Config(void);
void USART_Config(void);
void USART3_SendStr(uint16_t * sdrv,uint8_t len);
#endif
