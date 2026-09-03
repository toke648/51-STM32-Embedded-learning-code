#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "HelloWorld!");
	
	Serial_Init();
	
	Serial_SendByte(0x41);		//测试，发送一个数据0x41
	
	//指针只能是整数类型，因为保存的是地址
	uint8_t MyArray[] = {0x42,0x43,0x44,0x45};
	
	Serial_SendArray(MyArray, 4);
	
	Serial_SendString("HelloWorld!\r\n");
	
	Serial_SendNumber(123456, 6);
	
	printf("Num=%d\r\n", 666);
	
	
	
	//多个串口同时收发用sprintf
	char String[100];
	sprintf(String, "Num=%d\r\n", 666);
	Serial_SendString(String);
	
	Serial_Printf("你好世界\r\n");
	
	while(1)
	{

	}
}


