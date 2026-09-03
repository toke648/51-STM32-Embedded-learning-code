#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "LED.h"
#include "string.h"

uint16_t RxCount = 0;

//文本数据发送实现
int main(void)
{
	OLED_Init();
	LED_Init();
	Key_Init();
	Serial_Init();
	
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	
	while(1)
	{
		
		if (Serial_RxFlag == 1)				//只有接受到的数据标志位为1，才执行
		{
			 RxCount++;  					// 每次收到数据 +1
			
			OLED_ShowString(4, 1, "               "); //先擦除上个显示的数据
			OLED_ShowString(4, 1, Serial_RxPacket);
			
			OLED_ShowNum(1, 11, RxCount, 2);
			
			if (strcmp(Serial_RxPacket, "LED_ON") == 0)
			{
				LED1_ON();
				Serial_SendString("LED_ON_OK\r\n");
				
				OLED_ShowString(2, 1, "               ");
				OLED_ShowString(2, 1, "LED_ON_OK");
			}
			
			else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)
			{
				LED1_OFF();
				Serial_SendString("LED_OFF_OK\r\n");
				
				OLED_ShowString(2, 1, "               ");
				OLED_ShowString(2, 1, "LED_OFF_OK");
			}
			else
			{
				Serial_SendString("ERROR_COMMAND\r\n");
			
				OLED_ShowString(2, 1, "               ");
				OLED_ShowString(2, 1, "ERROR_COMMAND");
			}
		}
	}
}




