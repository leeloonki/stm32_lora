
#ifndef __LORA_H
#define __LORA_H
#include"stdint.h"
void Lora_Init(void);
void Lora_Config(void);
void USART3_SendStr(uint16_t * sdrv,uint8_t len);
#endif
