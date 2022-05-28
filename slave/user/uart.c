/*
 * @Date: 2022-05-26 13:36:30
 * @LastEditors: leeloonki 917708562@qq.com
 * @LastEditTime: 2022-05-28 08:29:33
 * @FilePath: \proj\slave\user\uart.c
 */
#include "uart.h"
#include "stm32f10x.h"
extern void delay_ms(int32_t ns);
void USART1_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//USART1_Tx -- PA9  复用推挽输出
	//USART1_Rx -- PA10 浮空输入
	//1.开A端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//串口时钟打开
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//串口输出RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//完成PA10设置

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//串口输出TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//复用开漏
	GPIO_Init(GPIOA, &GPIO_InitStructure);//完成PA9设置

	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	USART_Cmd(USART1,ENABLE);
}

void USART3_Config(void){
    GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO结构体
	USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//打开GPIOB时钟	//usart3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//打开USART3时钟
    //串口3 RX的GPIO配置为浮空输入 PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//选择输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//完成PB11设置
    //串口3 TX的GPIO配置为推挽复用模式  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//选择输出引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽模式
	GPIO_Init(GPIOB, &GPIO_InitStructure);//完成PB10设置

	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&USART_InitStructure);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_TC, ENABLE);
	USART_Cmd(USART3,ENABLE);
}

static int mfputc(int c)
{
	USART_SendData(USART3,c);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	return c;
}

void USART3_SendStr(uint16_t * sdrv,uint8_t len){
	uint8_t i=0;
	while(sdrv[i]!='\0'){
		mfputc(sdrv[i]);
		i++;
		if(i==len) break;
	}
}
