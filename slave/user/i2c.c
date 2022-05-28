/*
 * @Date: 2022-05-27 11:54:46
 * @LastEditors: leeloonki 917708562@qq.com
 * @LastEditTime: 2022-05-28 11:41:25
 * @FilePath: \slave\user\i2c.c
 */
#include "i2c.h"
#include "stm32f10x.h"

void I2c_Init(){
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//打开AFIO时钟
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
    // 生成通信起始信号
    I2C_GenerateSTART(I2C1,ENABLE);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
    // 发送通讯地址和通讯模式
    I2C_Send7bitAddress(I2C1,0x46,I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    // 发送数据
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


//   * 
//   * After checking on EV5 (start condition correctly released on the bus), the 
//   * master sends the address of the slave(s) with which it will communicate 
//   * (I2C_Send7bitAddress() function, it also determines the direction of the communication: 
//   * Master transmitter or Receiver). Then the master has to wait that a slave acknowledges 
//   * his address. If an acknowledge is sent on the bus, one of the following events will 
//   * be set:
//   * 
//   *  1) In case of Master Receiver (7-bit addressing): the I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED 
//   *     event is set.
//   *  
//   *  2) In case of Master Transmitter (7-bit addressing): the I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED 
//   *     is set
//   *  
//   *  3) In case of 10-Bit addressing mode, the master (just after generating the START 
//   *  and checking on EV5) has to send the header of 10-bit addressing mode (I2C_SendData() 
//   *  function). Then master should wait on EV9. It means that the 10-bit addressing 
//   *  header has been correctly sent on the bus. Then master should send the second part of 
//   *  the 10-bit address (LSB) using the function I2C_Send7bitAddress(). Then master 
//   *  should wait for event EV6. 
//   *     
//   */


/** 
  * @brief Communication events
  * 
  * If a communication is established (START condition generated and slave address 
  * acknowledged) then the master has to check on one of the following events for 
  * communication procedures:
  *  
  * 1) Master Receiver mode: The master has to wait on the event EV7 then to read 
  *    the data received from the slave (I2C_ReceiveData() function).
  * 
  * 2) Master Transmitter mode: The master has to send data (I2C_SendData() 
  *    function) then to wait on event EV8 or EV8_2.
  *    These two events are similar: 
  *     - EV8 means that the data has been written in the data register and is 
  *       being shifted out.
  *     - EV8_2 means that the data has been physically shifted out and output 
  *       on the bus.
  *     In most cases, using EV8 is sufficient for the application.
  *     Using EV8_2 leads to a slower communication but ensure more reliable test.
  *     EV8_2 is also more suitable than EV8 for testing on the last data transmission 
  *     (before Stop condition generation).
  *     
  *  @note In case the  user software does not guarantee that this event EV7 is 
  *  managed before the current byte end of transfer, then user may check on EV7 
  *  and BTF flag at the same time (ie. (I2C_EVENT_MASTER_BYTE_RECEIVED | I2C_FLAG_BTF)).
  *  In this case the communication may be slower.
  * 
  */