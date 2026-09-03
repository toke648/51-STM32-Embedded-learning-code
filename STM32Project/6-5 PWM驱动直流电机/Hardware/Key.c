#include "stm32f10x.h"                  // Device header
#include "Delay.h" 

//uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
//uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

// uint8_t GPIO_ReadInputDataBit
// uint8_t 返回值为 uint8_t 读取函数时使用
// 用于读取输入寄存器某一个端口的输入值(GPIOx, GPIO_Pin); 用来指定某一个端口
// uint16_t GPIO_ReadInputData
// 读取指定外设，返回uint16_t，是一个16位的数据，每一位代表一个端口值
// uint8_t GPIO_ReadOutputDataBit
// 读取输出数据寄存器的某一位，不是用来读取端口的输入数据
// uint16_t GPIO_ReadOutputData
// 用于读取整个输出寄存器

void Key_Init(void){
	// 使用GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// IPU（in Pull Up）	上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

uint8_t Key_GetNum(void){
	uint8_t KeyNum = 0;

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0);
		Delay_ms(20);
		KeyNum = 1; // 传递判断数据
	}
//	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0){ //Pin_11
//		Delay_ms(20);
//		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0); // GPIO_ReadInputDataBit 
//		Delay_ms(20);
//		KeyNum = 2;
//	}
	
	return KeyNum;
}
