#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "Buzzer.h"
#include "LightSensor.h"

typedef enum{
	A = (uint32_t)0x00000004,
	B = (uint32_t)0x00000008,
	C = (uint32_t)0x00000010,
	D = (uint32_t)0x00000020,
	E = (uint32_t)0x00000040,
	F = (uint32_t)0x00000080,
	G = (uint32_t)0x00000100,
}Rcc_GPIO;

//typedef enum {
//    PIN_0 = 0x0001,  // 1
//    PIN_1 = 0x0002,  // 2
//    PIN_2 = 0x0004,  // 4
//	PIN_3 = 0x0008,
//    PIN_4 = 0x00010,  // 8
//	PIN_5 = 0x00020,
//	PIN_6 = 0x00040,
//	PIN_7 = 0x00080,
//	PIN_8 = 0x00100,
//	PIN_9 = 0x00200,
//	PIN_10 = 0x00400,
//	PIN_11 = 0x0800,
//	PIN_12 = 0x1000,
//	PIN_13 = 0x2000,
//	PIN_14 = 0x4000,
//	PIN_15 = 0x8000,
//	PIN_16 = 0xFFFF,
//	
//} Pin_Type;

//	Pin_Type pin;
//	pin = Pin_1;

#define Pin_0 (uint16_t)0x0001
#define Pin_1 (uint16_t)0x0002
#define Pin_2 (uint16_t)0x0004
#define Pin_3 (uint16_t)0x0008
#define Pin_4 (uint16_t)0x0010
#define Pin_5 (uint16_t)0x0020
#define Pin_6 (uint16_t)0x0040
#define Pin_7 (uint16_t)0x0080
#define Pin_8 (uint16_t)0x0100
#define Pin_9 (uint16_t)0x0200
#define Pin_10 (uint16_t)0x0400
#define Pin_11 (uint16_t)0x0800
#define Pin_12 (uint16_t)0x1000
#define Pin_13 (uint16_t)0x2000
#define Pin_14 (uint16_t)0x4000
#define Pin_15 (uint16_t)0x8000
#define Pin_All (uint16_t)0xFFFF  /*!< All pins selected */

uint8_t KeyNum;

int main(void)
{
    Buzzer_Init();
	LightSensor_Init();
	
	while(1)
	{
		if(LightSensor_Get() == 1){
			Buzzer_ON();
//			Delay_ms(500); // 暂停五十毫秒
		}else{
			Buzzer_OFF();
//			Delay_ms(500); 
		}

	}
}
