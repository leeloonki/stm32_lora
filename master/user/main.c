// 主板
#include "main.h"
#include "string.h"
#include "usart.h"
#include "lora.h"

uint16_t recv[30];
uint8_t rc = 0;
uint16_t data = 0;



int main()
{
	JTAG_SWD_Config();//关闭JTAG功能，打开SWD功能
	USART_Config();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	Lora_Init();
	//主循环轮询判断串口3是否接收到数据
	while(1){
		if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)==SET){
			data = USART_ReceiveData(USART3);
			USART_ITConfig(USART1, USART_IT_TC, ENABLE);
		}
	}
}

void echo(){
	uint8_t choice = recv[0];
	if(choice=='0'||choice=='1') {
		configPara(choice);
	}
	else if(choice == '2'){
		sendRequest();
	}else{
		
	}
	memset(recv,'\0',sizeof(recv));
	rc=0;
}

// 输入：0 # 	--配置工作参数
// 输入：1 # 	--查看工作参数
// 输入：2 # 	--获取光照
// 输入：3 #	--获取温湿度
void USART1_IRQHandler(){
	uint16_t receive_temp;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET){
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		receive_temp=USART_ReceiveData(USART1);
		if(receive_temp!='#'){
			recv[rc++]=receive_temp;
		}else{
			echo();
		}
	}
	if(USART_GetITStatus(USART1,USART_IT_TC)==SET){
		USART_ClearITPendingBit(USART1,USART_IT_TC);
		USART_SendData(USART1,data);
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	}
}


