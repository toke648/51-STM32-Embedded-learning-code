#include <REGX52.H>
#include "I2C.h"

#define At24C02_ADDRESS		0xA0 // A0 1010 0000 B0 1011 0000

//void AT24C02_WriteCheck(){

//}

/**
  * @brief AT24C02 写入一个字节
  * @param WordAddress 字节地址（0x00-0xFF）
  * @param Data 要写入的数据
  * @retval 无
  */
void AT24C02_WriteByte(unsigned char WordAddress,unsigned char Data){	
	I2C_Start(); //启动总线
	I2C_SendByte(At24C02_ADDRESS); //写入位
	I2C_ReceiveAck(); //接受应答
	I2C_SendByte(WordAddress);//写入自定义地址
	//1为写入，0为等待
	I2C_ReceiveAck();
	I2C_SendByte(Data); //写入字节
	
	I2C_ReceiveAck();
	I2C_Stop();
	
//	if(Ack==1)P2=0x00; //判断是否有应答
}

/**
  * @brief AT24C02读取一个字节 
  * @param WordAddress 字节地址（0x00-0xFF）共255个
  * @retval Data 返回读取到的数据
  */
unsigned char AT24C02_ReadByte(unsigned char WordAddress){
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(At24C02_ADDRESS);
	I2C_ReceiveAck();
	I2C_SendByte(WordAddress);//写入地址
	I2C_ReceiveAck();
	
	I2C_Start();
	//写入+读取 At24C02_ADDRESS+R
	I2C_SendByte(At24C02_ADDRESS|0x01);
	I2C_ReceiveAck();
	
	Data=I2C_ReceiveByte(); //接收一个字节的数据
	I2C_SendAck(1);
	I2C_Stop();
	
	return Data;
}