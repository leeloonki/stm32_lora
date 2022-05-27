#include "lora.h"
#include "stm32f10x.h"
uint16_t para[6] = {0xC0,0x00,0x02,0x1A,0x3C,0x40};	//默认工作参数 地址0x01 频段0x3c
uint16_t para_saved[6] = {0xC1,0xC1,0xC1};
uint16_t para_request[6] = {0xff,0xff,0x3c};	//0x01表示希望获取光照
extern void delay_ms(int32_t ns);
void Lora_Init(){
    GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO结构体
    // m0  PB0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//选择输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;//开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//完成PA4设置
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);//点亮
    //m1   PA5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//选择输出引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;//开漏输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//完成PA5设置
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);//点亮
}

void Lora_Config(){
	// 休眠模式下
	GPIO_SetBits(GPIOA, GPIO_Pin_4);//M0
	GPIO_SetBits(GPIOA, GPIO_Pin_5);//M1
	// 空闲200ms等待进入休眠配置模式
	delay_ms(200);
	USART3_SendStr(para,sizeof(para)/sizeof(uint16_t));
}

void sendInfo(){
	//M0=0,M1=0,一般工作模式
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);//M0
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);//M1
  delay_ms(200);	
	//USART3_SendStr(para_request,sizeof(para_request));//一般工作模式下从串口3发出数据，main函数等待接收串口3数据。
}

