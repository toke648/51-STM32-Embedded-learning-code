#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "MyI2C.h"
#include "MPU6050.h"
#include "OLED.h"

int16_t AX,AY,AZ,GX,GY,GZ;

int main(void)
{
	uint8_t ID = 0;

	OLED_Init();
	MyI2C_Init();
	MPU6050_Init();
		
	OLED_ShowString(1, 1, "MPU6050 ID:");
	ID = MPU6050_GetID();
	
	OLED_ShowHexNum(1, 12, ID, 2);
	
	while(1)
	{
		//由于传递的是指针数据，因此需要对寄存器取地址操作
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
		
	}
}

//	OLED_ShowString(1, 1, "MPU6050");

//	MPU6050_WriteReg(0x68, 0x00);
//	
//	MPU6050_WriteReg(0x19, 0x66); 
//	
//	ID = MPU6050_ReadReg(0x19);
//	
//	OLED_ShowHexNum(2, 1, ID, 2);
	
//	
//	
//	MyI2C_Start();
//	MyI2C_SendByte(0xD2);		//1101 0000
//	uint8_t Ack = MyI2C_ReceiveAck();	//0为收到应答，1为未收到应答
//	MyI2C_Stop();
//	
//	OLED_ShowNum(2, 1, Ack, 3);
	
	
//	int i,Num;

		//通道扫描检测
//		for(i=0;i<=256;i++)
//		{
//			MyI2C_Start();
//			Num = 0x01 + i;
//			MyI2C_SendByte(Num);		//1101 0000
//			uint8_t Ack = MyI2C_ReceiveAck();	//0为收到应答，1为未收到应答
//			MyI2C_Stop();
//			
//			OLED_ShowNum(2, 1, Ack, 3);
//			
//			if (Ack == 0)
//			{	
//				OLED_ShowHexNum(2, 8, Num, 3);
//				
//				Delay_ms(5000);
//			}
//			
//			Delay_ms(10);
//		}
