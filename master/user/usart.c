/*
 * @Date: 2022-05-27 08:04:07
 * @LastEditors: leeloonki 917708562@qq.com
 * @LastEditTime: 2022-05-28 08:21:40
 * @FilePath: \proj\master\user\usart.c
 */
#include "usart.h"
static void NVIC_Configuration(void);
// 配置串口1中断
void USART1_Config(void){
    GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO结构体
	USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//打开GPIOA时钟   	//usart1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//打开AFIO时钟	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//打开USART时钟
    //串口1 RX的GPIO配置为浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//选择输出引脚 rx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//完成PA10设置
    //串口1 TX的GPIO配置为开漏复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//选择输出引脚 tx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//开漏模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);//完成PA9设置

    USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
    // 串口初始化配置
	USART_Init(USART1,&USART_InitStructure);
    // 串口中断优先级配置
    NVIC_Configuration();
    // 使能串口接收中断
	// 使能串口
    USART_Cmd(USART1,ENABLE);
}
void USART3_Config(void){
    GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO结构体
	USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//打开GPIOB时钟	//usart3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//打开USART3时钟
    //串口3 RX的GPIO配置为浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//选择输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//完成PB11设置
    //串口3 TX的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//选择输出引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//推挽模式
	GPIO_Init(GPIOB, &GPIO_InitStructure);//完成PB10设置

	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&USART_InitStructure);
	USART_Cmd(USART3,ENABLE);
}

void USART_Config(void){
    USART1_Config();
    USART3_Config();
}


static void NVIC_Configuration(void){
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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
