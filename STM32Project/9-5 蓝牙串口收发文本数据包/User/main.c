#include "stm32f10x.h"
#include "Delay.h" 
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "LED.h"
#include "string.h"

uint16_t RxCount = 0;

int main(void)
{
    OLED_Init();
    LED_Init();
    Key_Init();
    Serial_Init();
    
    OLED_ShowString(1, 1, "TxPacket");
    OLED_ShowString(3, 1, "RxPacket");
    OLED_ShowString(4, 1, "Data:");
	
	Serial_SendString("Hello");
	Serial_Printf("World");
    
    while(1)
    {
		if (Serial_GetRxFlag() == 1)
		{
			RxCount++;
			OLED_ShowString(4, 6, "                ");
			OLED_ShowString(4, 6, Serial_RxPacket);
			OLED_ShowNum(1, 11, RxCount, 2);
			
			// ✅ 先判断文本命令
			if (strcmp(Serial_RxPacket, "LED_ON") == 0)
			{
				LED1_ON();
				Serial_SendString("LED_ON_OK\r\n");
				OLED_ShowString(2, 1, "LED_ON_OK    ");
			}
			else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)
			{
				LED1_OFF();
				Serial_SendString("LED_OFF_OK\r\n");
				OLED_ShowString(2, 1, "LED_OFF_OK   ");
			}
			// ✅ 新增：判断 HEX 格式命令
			else if (strcmp(Serial_RxPacket, "ON") == 0)   // 发送 "@ON\r\n"
			{
				LED1_ON();
				Serial_SendString("LED_ON_OK\r\n");
				OLED_ShowString(2, 1, "HEX_ON_OK    ");
			}
			else if (strcmp(Serial_RxPacket, "OFF") == 0)  // 发送 "@OFF\r\n"
			{
				LED1_OFF();
				Serial_SendString("LED_OFF_OK\r\n");
				OLED_ShowString(2, 1, "HEX_OFF_OK   ");
			}
			else
			{
				Serial_SendString("ERROR_COMMAND\r\n");
				OLED_ShowString(2, 1, "ERROR_CMD    ");
			}
		}
    }
}