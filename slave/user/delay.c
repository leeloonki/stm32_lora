#include "stdint.h"
void delay_ms(int32_t ns){
	int32_t i;
	while(ns--){
		i=7500;
		while(i--);
	}
}
