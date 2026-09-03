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
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	LED_Init();			//LED初始化
	Serial_Init();		//串口初始化
	
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	
	while(1)
	{
		
		if (Serial_RxFlag == 1)		//如果接收到数据包
		{
			 RxCount++;  					// 每次收到数据 +1
			
			OLED_ShowString(4, 1, "               "); //先擦除上个显示的数据
			OLED_ShowString(4, 1, Serial_RxPacket);
			
			OLED_ShowNum(1, 11, RxCount, 2);
			
			/*将收到的数据包与预设的指令对比，以此决定将要执行的操作*/
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
				Serial_SendString("ERROR_COMMAND\r\n");			//串口回传一个字符串ERROR_COMMAND
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ERROR_COMMAND");			//OLED清除指定位置，并显示ERROR_COMMAND
			}
			
			Serial_RxFlag = 0;			//处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
		}
	}
}




