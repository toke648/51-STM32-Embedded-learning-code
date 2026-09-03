#include "stm32f10x.h"                  // Device header

// stm32f10x_rcc.h中，常用的三个时钟控制函数
// RCC APB2外设时钟控制、RCC APB1外设时钟控制
// RCC APB
// void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);
// void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
// void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);


// stm32f10x_gpio.h文件包含了所有外设的可设置函数
// 常用的有

// GPIO_DeInit 复位函数

//void GPIO_DeInit(GPIO_TypeDef* GPIOx);

// GPIO_AFIODeInit 复位

//void GPIO_AFIODeInit(void);

// GPIO_Init 用结构体的参数来初始化GPIO口

//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);

// GPIO_StructInit 给以GPIO口默认值

//void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);


// GPIO的读取函数

//uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

//uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);

//uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

//uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);


// GPIO的写函数

// GPIO_SetBits 用于把指定的端口设置为高电平

//void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

// GPIO_ResetBits 用于把指定的端口设置为低电平

//void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

// GPIO_WriteBit 根据第三个参数的值设置指定端口

//void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);

//  GPIO_Write 根据第一个参数GPIOx选择外设

// 第二个参数ProtValue函数，可以同时对16个端口进行写入操作

//void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);


// GPIO的几种输出模式的设置
//typedef enum
//{ GPIO_Mode_AIN = 0x0,
//  GPIO_Mode_IN_FLOATING = 0x04,
//  GPIO_Mode_IPD = 0x28,
//  GPIO_Mode_IPU = 0x48,
//  GPIO_Mode_Out_OD = 0x14,
//  GPIO_Mode_Out_PP = 0x10,
//  GPIO_Mode_AF_OD = 0x1C,
//  GPIO_Mode_AF_PP = 0x18
//}GPIOMode_TypeDef;

//AIN（Analog IN）	模拟输入

//IN_FLOATING	浮空输入

//IPD（in Pull Down）	下拉输入

//IPU（in Pull Up）	上拉输入

//Out_OD（Out Open Drain）	开漏输出

//Out_PP（Out Push Pull）	推挽输出

//AF_OD（Atl Open Drain）	复用开漏

//AF_PP（Atl Push Pull）	复用推挽


void Delay(uint32_t count){
	while(count--);
}

// GPIO初始化
int main(void)
{
	int i,j;
	
	// GPIO初始化结构体地址
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 启动时钟控制
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 因为上一步的时钟控制引脚是GPIOA，因此下面也填写GPIOA
	// 定义局部变量参数
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// 引脚设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	// 输出频率控制
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// 对GPIO结构体地址进行指针调取操作&GPIO_InitStructure
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 根据函数介绍，第一个参数GPIO_Pin_x，x0-15
	//GPIO_ResetBits(GPIOA, GPIO_Pin_0); //高电平
	
	//GPIO_SetBits(GPIOA, GPIO_Pin_0); //低电平
	
	// 使用GPIO_WriteBit直接控制，Bit_SET 低电平，Bit_RESET 高电平
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	
	while(1)
	{

	}
}
