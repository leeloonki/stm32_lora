#include "i2c.h"
#include "stm32f10x.h"

void I2c_Init(){
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//打开GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//打开GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//打开AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//打开USART时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);//打开I2C时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//I2C SCL
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//完成PA2设置

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//I2C SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//设置I/O反转速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//完成PA2设置

    I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed=20000;
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1=0x5C;
	I2C_Cmd(I2C1,ENABLE);
	I2C_Init(I2C1,&I2C_InitStructure);
	I2C_AcknowledgeConfig(I2C1,ENABLE);
}

uint16_t getliuming(){
    uint8_t fs1;
    uint8_t fs2;
    fs1=0;
    fs2=0;
    uint16_t liuming;
    I2C_GenerateSTART(I2C1,ENABLE);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1,0x46,I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1,0x21);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1,ENABLE);
    delay_ms(800);
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1,ENABLE);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1,0x46,I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
    fs1=I2C_ReceiveData(I2C1);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
    fs2=I2C_ReceiveData(I2C1);
    I2C_GenerateSTOP(I2C1,ENABLE);
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    liuming=(fs1<<8)|fs2;
    return liuming;
}
