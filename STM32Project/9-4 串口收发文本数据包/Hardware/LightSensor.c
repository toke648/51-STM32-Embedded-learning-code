#include "stm32f10x.h"                  // Device header

// 通过定义函数，后面就可以主程序文件里方便的使用和更改了
//void LED_Init(unsigned char rcc_GPIO[], unsigned int pin_GPIO)
void LightSensor_Init(void)
{
	// 1.开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// 2.直接引用GPIO_InitTypeDef typedef定义的类型
	GPIO_InitTypeDef GPIO_InitStructure;
	// 3.启动模式使用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入（输入端接电）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // 使用引脚（多引脚用或连接）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出频率50Hz
	// 4.引脚初始化
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// 5.初始化后，如果不操作LED，LED自动熄灭
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	
}

uint8_t LightSensor_Get(void){
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);

}

//void LightSensor_Turn(void){
//	// 如果按钮没有被按下，LED灯就是灭，否则为亮
//	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0){
//		GPIO_SetBits(GPIOB, GPIO_Pin_12);
//	}
//	else{
//		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
//	}

//}
