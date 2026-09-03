#include "stm32f10x.h"                  // Device header

// 通过定义函数，后面就可以主程序文件里方便的使用和更改了
//void LED_Init(unsigned char rcc_GPIO[], unsigned int pin_GPIO)
void LED_Init(unsigned int rcc, uint16_t pins)
{
	// 1.开启时钟
	RCC_APB2PeriphClockCmd(rcc, ENABLE);

	// 2.直接引用GPIO_InitTypeDef typedef定义的类型
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 3.启动模式使用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = pins; // 使用引脚（多引脚用或连接）
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
