#include "stm32f10x.h"                  // Device header

typedef enum{
	A = (uint32_t)0x00000004,
	B = (uint32_t)0x00000008,
	C = (uint32_t)0x00000010,
	D = (uint32_t)0x00000020,
	E = (uint32_t)0x00000040,
	F = (uint32_t)0x00000080,
	G = (uint32_t)0x00000100,
}Rcc_GPIO;

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

// 通过定义函数，后面就可以主程序文件里方便的使用和更改了
//void LED_Init(unsigned char rcc_GPIO[], unsigned int pin_GPIO)
void LED_Init(void)
{
	Rcc_GPIO rcc;
	rcc = A;
	
	// 1.开启时钟
	RCC_APB2PeriphClockCmd(rcc, ENABLE);

	// 2.直接引用GPIO_InitTypeDef typedef定义的类型
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 3.启动模式使用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Pin_0 | Pin_1; // 使用引脚（多引脚用或连接）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出频率50Hz
	
	// 4.引脚初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 初始化后，如果不操作LED，LED自动熄灭
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
	
}


//新建函数，用于点亮LED
void LED0_ON(void){
	// 设置引脚高电平
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);

}

void LED0_OFF(void){
	// 设置引脚低电平
	GPIO_SetBits(GPIOA, GPIO_Pin_0);

}

void LED0_Turn(void){
	// 如果按钮没有被按下，LED灯就是灭，否则为亮
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0){
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
	}
	else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	}
}

void LED1_Turn(void){
	// Pin1
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0){
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}
}


void LED1_ON(void){
	// 设置引脚高电平
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);

}

void LED1_OFF(void){
	// 设置引脚低电平
	GPIO_SetBits(GPIOA, GPIO_Pin_1);

}
