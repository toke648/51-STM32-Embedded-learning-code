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

void Key_Init(unsigned int rcc, uint16_t pins){
	// 使用GPIOB
	RCC_APB2PeriphClockCmd(rcc, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// IPU（in Pull Up）	上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = pins;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

uint8_t Key_GetNum(void){
	uint8_t KeyNum = 0;

	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		Delay_ms(20);
		KeyNum = 1; // 传递判断数据
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0){ //Pin_11
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0); // GPIO_ReadInputDataBit 
		Delay_ms(20);
		KeyNum = 2;
	}
	
	return KeyNum;
}
