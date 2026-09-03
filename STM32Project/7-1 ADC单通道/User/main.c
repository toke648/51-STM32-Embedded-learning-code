#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;
float Voltage;


/*
数模转换，将数字电流转换为模拟电流
*/
int main(void)
{
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1, 1, "ADValue:");
	OLED_ShowString(2, 1, "Voltage:0.00V");
	
	while(1)
	{
		ADValue = AD_GetValue();
		
		//先对整型ADValue类型强转
		//提高检测精度
		Voltage = (float)ADValue / 4095 * 3.3;		//将0-4095的数值范围变换到3.3
		OLED_ShowNum(1, 9, ADValue, 4);
		OLED_ShowNum(2, 9, Voltage, 1);
		OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2);
		
		Delay_ms(100);
	}
}
