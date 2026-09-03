#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h"
//#include "CountSensor.h"
#include "Encoder.h"


//2^16 65536个字节数
uint16_t Num;

int main(void)
{
	OLED_Init();
	Encoder_Init();
//	CountSensor_Init();

	OLED_ShowString(1, 1, "Num");
	
	while(1)
	{
		Num += Encoder_Get();
		OLED_ShowSignedNum(1,5,Num,5);
		Delay_ms(50);
	}
}
