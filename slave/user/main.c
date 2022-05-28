#include "main.h"
#include "string.h"
#include "i2c.h"
#include "uart.h"
#include "lora.h"

uint16_t guangzhao;
extern uint16_t para_response[6];
int main()
{
	JTAG_SWD_Config();//关闭JTAG功能，打开SWD功能
	USART1_Config();
	USART3_Config();
	I2c_Init();
	Lora_Init();
	Lora_Config();//配置工作参数
	while(1);
}

void USART3_IRQHandler(){
	uint16_t receive_temp;
	if(USART_GetITStatus (USART3,USART_IT_RXNE)==SET){
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		receive_temp=USART_ReceiveData(USART3);
		if(receive_temp==0x02){//获取光照 
			// sprintf(send,"光照强度为:%d",getlimingstr());
			guangzhao = getliuming();
			USART_ITConfig(USART3, USART_IT_TC, ENABLE);
		}else {
			// 获取温湿度
		}
	}
	if(USART_GetITStatus (USART3,USART_IT_TC)==SET){
		USART_ClearITPendingBit(USART3,USART_IT_TC);
		para_response[4] = guangzhao;
		USART3_SendStr(para_response,sizeof(para_response)/sizeof(uint16_t));
	}else{

	}
}



