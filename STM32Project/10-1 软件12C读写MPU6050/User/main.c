#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "MyI2C.h"

#include "OLED.h"

int main(void)
{
	OLED_Init();
	My12C_Init();
	
	OLED_ShowString(1, 1, "MPU6050");
	
	MyI2C_Start();
	MyI2C_SendByte(0xD0);		//1101 0000
	uint8_t Ack = MyI2C_ReceiveAck();
	MyI2C_Stop();
	
	OLED_ShowNum(1, 1, Ack, 3);
	
	while(1)
	{

	}
}
