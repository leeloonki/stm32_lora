#include "lora.h"
#include "stm32f10x.h"

#include "usart.h"
uint16_t para[6] = {0xC0,0x00,0x01,0x1A,0x3C,0x40};	//默认工作参数 地址0x01 频段0x3c
uint16_t para_saved[6] = {0xC1,0xC1,0xC1};
uint16_t para_request[6] = {0xff,0xff,0x3c};	//0x01表示希望获取光照

void delay(uint32_t ns)
{
	uint16_t i;
	while(ns>0){
		ns--;
		i=10000;
		while(i>0) i--;
	}
}

void Lora_Init(){
    GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO结构体
    // m0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//选择输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;//开漏输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//完成PA4设置
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);//点亮
    //m1 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//选择输出引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;//开漏输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//完成PA5设置
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);//点亮
}


void configPara(int number){
	// 休眠模式下
	GPIO_SetBits(GPIOA, GPIO_Pin_4);//M0
	GPIO_SetBits(GPIOA, GPIO_Pin_5);//M1
	// 空闲200ms等待进入休眠配置模式
	delay(200);
	// 配置工作参数
	if(number=='0'){
		// 配置为默认参数 para[]
		// 使用串口3 波特率9600(usart.c中配置) 8N1通过特定指令格式设置模块工作参数。
		USART3_SendStr(para,sizeof(para)/sizeof(uint16_t));
	}else if(number == '1'){
		// 查看工作参数
		USART3_SendStr(para_saved,sizeof(para_saved)/sizeof(uint16_t));
		//main中轮询检查串口3接收标志位
	}
}

void sendRequest(){
	//M0=0,M1=0,一般工作模式
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);//M0
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);//M1
    delay(200);	
	para_request[4] = 0x02;			//0x02表示获取光照
	USART3_SendStr(para_request,sizeof(para_request));//一般工作模式下从串口3发出数据，main函数等待接收串口3数据。
}
