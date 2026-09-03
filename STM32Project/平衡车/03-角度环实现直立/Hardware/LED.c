#include "stm32f10x.h"                  // Device header

// 通过定义函数，后面就可以主程序文件里方便的使用和更改了
//void LED_Init(unsigned char rcc_GPIO[], unsigned int pin_GPIO)
void LED_Init(void)
{
	// 1.开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// 2.直接引用GPIO_InitTypeDef typedef定义的类型
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 3.启动模式使用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // 使用引脚（多引脚用或连接）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出频率50Hz
	
	// 4.引脚初始化
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 初始化后，如果不操作LED，LED自动熄灭
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	
}


//新建函数，用于点亮LED
void LED_ON(void){
	// 设置引脚高电平
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);

}

void LED_OFF(void){
	// 设置引脚低电平
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED_Turn(void){
	// 如果按钮没有被按下，LED灯就是灭，否则为亮
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0){
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	}
	else{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}
}
